#include <unistd.h>
#include <stddef.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <arpa/inet.h>
#include <xtimer.h>

#define EMCUTE_ID     (1U)
#define EMCUTE_PRIO   (THREAD_PRIORITY_MAIN - 1)
#define MQTT_PORT     1883
#define MQTT_ADDRESS  "47.102.103.1"
#define TOPIC_NAME    "esp32/status"
#define PUB_INTERVAL  (5U * US_PER_SEC)  // publish every 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];

static emcute_topic_t topic;
static emcute_sub_t subscription;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    // Handle incoming publish data
}

static void *publisher_thread(void *arg)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        const char *status = "work";  // or "not work", set according to your actual logic
        if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        } else {
            puts("Data published");
        }
        xtimer_periodic_wakeup(&last_wakeup, PUB_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    msg_t queue[EMCUTE_ID + 1];
    msg_init_queue(queue, EMCUTE_ID + 1);

    /* setup our subscription */
    subscription.cb = on_pub;
    subscription.topic.name = TOPIC_NAME;

    if (emcute_sub(&subscription, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    } else {
        puts("Subscribed successfully");
    }

    /* connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_PORT };
    inet_pton(AF_INET, MQTT_ADDRESS, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    } else {
        puts("Connected to broker");
    }

    /* get topic ID */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    } else {
        puts("Topic registered");
    }

    /* create a thread to publish data */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher_thread, NULL, "publisher");

    return 0;
}