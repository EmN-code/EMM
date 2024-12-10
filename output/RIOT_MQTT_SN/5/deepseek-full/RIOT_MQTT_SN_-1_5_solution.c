
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_udp_ep_t broker_ep = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

int main(void)
{
    // Convert IP address string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &broker_ep.addr.ipv4) != 1) {
        printf("Error in IP address conversion\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    char state[] = "work or not work";
    while (1) {
        res = emcute_pub(&topic, state, sizeof(state) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}
