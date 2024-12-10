#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include "emcute.h"
#include "net/ipv4/addr.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5LU * US_PER_SEC)  // Publish every 5 seconds

static emcute_topic_t topic;

void *publish_thread(void *arg) {
    (void)arg;
    while(1) {
        const char *msg = "work";
        printf("Publishing: %s\n", msg);
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing");
        }
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    // MQTT-SN connection setup
    sock_udp_ep_t gw = {.port = BROKER_PORT};
    gw.family = AF_INET;

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_IP) == NULL) {
        printf("Error parsing broker IP\n");
        return 1;
    }

    if (emcute_con((sock_udp_ep_t *)&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to broker\n");
        return 1;
    }

    // Topic registration
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic\n");
        return 1;
    }

    // Start publish thread
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish_thread");

    // Infinite loop to keep main thread alive
    while (1) {
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}