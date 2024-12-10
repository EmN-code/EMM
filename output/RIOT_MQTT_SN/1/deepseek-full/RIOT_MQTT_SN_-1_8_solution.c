
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV4_EP_ANY;

void *publish_state(void *arg) {
    (void)arg;
    const char *states[] = {"work", "not work"};
    unsigned state_index = 0;

    while (1) {
        // Publish the state
        const char *state = states[state_index];
        int res = emcute_pub(&topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %s\n", (res == EMCUTE_NOGW) ? "Not connected to gateway" : "Other error");
        } else {
            printf("Published state: %s\n", state);
        }

        // Toggle state
        state_index = (state_index + 1) % 2;

        // Sleep for 5 seconds
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize the UDP endpoint for the gateway
    gw_ep.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %s\n", (res == EMCUTE_NOGW) ? "Not connected to gateway" : "Other error");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %s\n", (res == EMCUTE_NOGW) ? "Not connected to gateway" : "Other error");
        return 1;
    }

    // Create a thread to publish the state every 5 seconds
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publish_state");

    // Main loop
    while (1) {
        // Keep the main thread alive
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
