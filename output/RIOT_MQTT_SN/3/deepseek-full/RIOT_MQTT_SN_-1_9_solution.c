
#include "dht_params.h"
#include "stdbool.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "mutex.h"
#include "net/emcute.h"
#include "dht.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "stdio.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t remote;

int main(void)
{
    // Initialize EMCUTE
    emcute_init();

    // Set up the remote endpoint for the MQTT-SN broker
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    // Publish state every 5 seconds
    const char *states[] = {"work", "not work"};
    int state_index = 0;

    while (1) {
        // Publish the state
        res = emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        } else {
            printf("Published state: %s\n", states[state_index]);
        }

        // Toggle state
        state_index = (state_index + 1) % 2;

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
