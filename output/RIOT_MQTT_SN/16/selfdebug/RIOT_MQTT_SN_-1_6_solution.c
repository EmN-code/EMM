#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t emcute_topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

// Callback function for published messages (required by emcute, not used here)
static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    // Initialize the UDP endpoint
    int res = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error setting broker address\n");
        return 1;
    }

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to the broker\n");
        return 1;
    }

    // Register a topic
    emcute_topic.name = "state";
    if (emcute_reg(&emcute_topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    // Publish the state every 5 seconds
    while (1) {
        res = emcute_pub(&emcute_topic, "work or not work", strlen("work or not work"), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish data\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}