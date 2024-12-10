#include <net/sock/udp.h>
#include <net/emcute.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"

#define MQTT_PORT          (1883)
#define MQTT_BROKER_ADDR   "47.102.103.1"
#define PUB_INTERVAL       (5U) // publish every 5 seconds
#define EMCUTE_ID          "riot"
#define TOPIC_NAME         "device/state"

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    int counter = 0;

    while (1) {
        char data[32];
        snprintf(data, sizeof(data), "state: %s", (counter++ % 2) ? "work" : "not work");
        t.name = TOPIC_NAME;
        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("error: unable to obtain topic ID\n");
            continue;
        }
        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Published: %s to topic: %s\n", data, TOPIC_NAME);
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    /* initialize the network interface */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR) == NULL) {
        printf("error: unable to parse address\n");
        return 1;
    }

    /* connect to the MQTT broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT broker\n");
        return 1;
    }

    /* start the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher, NULL, "publisher");

    /* the actual network layer is run within the main thread */
    emcute_run(MQTT_PORT, EMCUTE_ID);

    return 0;
}
