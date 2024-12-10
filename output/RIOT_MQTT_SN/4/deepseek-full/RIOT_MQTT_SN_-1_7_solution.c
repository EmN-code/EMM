
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <dht.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <mma8x5x.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };

static void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("State published: %s\n", state);
    }
}

int main(void) {
    // Initialize the UDP endpoint for the gateway
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
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
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}
