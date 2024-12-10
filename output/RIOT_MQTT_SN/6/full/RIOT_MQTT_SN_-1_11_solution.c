#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <thread.h>
#include <string.h>
#include <msg.h>
#include <emcute.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>

#define EMCUTE_PORT     1883
#define MQTT_TOPIC      "state"
#define MQTT_BROKER_IP  "47.102.103.1"
#define SLEEP_DURATION  (5U * 1000 * 1000) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];
static emcute_topic_t topic;

static int _publish_state(emcute_topic_t *topic, const char *message) {
    int res = emcute_pub(topic, message, strlen(message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
        return -1;
    }
    printf("Published: %s on topic: %s\n", message, topic->name);
    return 0;
}

static void *_emcute_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    // prepare the topic
    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }
    topic = t;

    // ensure continuous connection and message publishing
    while (1) {
        int rc = _publish_state(&topic, "work");
        if (rc != 0) {
            printf("Retrying to publish...\n");
            ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
            continue;
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // connect to gateway
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to %s\n", MQTT_BROKER_IP);
            break;
        }
        printf("Connection failed, retrying in 1 second...\n");
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    // initialize and run emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _emcute_thread, NULL, "emcute-thread");

    // keep the main thread alive
    while (1) {
        thread_sleep();
    }
    return 0;
}