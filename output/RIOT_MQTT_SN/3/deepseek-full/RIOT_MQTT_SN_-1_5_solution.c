
#include "dht_params.h"
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include <stdlib.h>
#include "mutex.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include <stdatomic.h>
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    }
}

int main(void) {
    // Initialize EMCUTE
    emcute_init();

    // Convert IP address string to byte representation
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

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

    // Main loop to publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();
    return 0;
}
