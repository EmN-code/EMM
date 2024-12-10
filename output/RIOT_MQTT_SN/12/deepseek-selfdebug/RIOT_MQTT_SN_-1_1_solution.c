
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");
    gw.family = AF_INET;

    if (emcute_con(&gw, true, EMCUTE_ID, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC_NAME;
    if (emcute_sub(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    while (1) {
        char payload[20];
        snprintf(payload, sizeof(payload), "work");

        if (emcute_pub(&topic, payload, strlen(payload), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Published: work");
        }

        xtimer_usleep(PUBLISH_INTERVAL);

        snprintf(payload, sizeof(payload), "not work");

        if (emcute_pub(&topic, payload, strlen(payload), EMCUTE_QOS_0) != EMCUTE_OK) {
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
    puts("Starting MQTT-SN client");

    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
