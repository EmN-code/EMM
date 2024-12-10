
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define EMCUTE_PORT         (1883)
#define EMCUTE_ID           "riot_esp32"
#define TOPIC_NAME          "state"
#define PUBLISH_INTERVAL    (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    puts("Connected to MQTT-SN broker");

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    puts("Topic registered");

    char payload[20];
    while (1) {
        snprintf(payload, sizeof(payload), "work");
        if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Published: work");
        }

        xtimer_usleep(PUBLISH_INTERVAL);

        snprintf(payload, sizeof(payload), "not work");
        if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Published: not work");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(emcute_stack, sizeof(emcute_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    return 0;
}
