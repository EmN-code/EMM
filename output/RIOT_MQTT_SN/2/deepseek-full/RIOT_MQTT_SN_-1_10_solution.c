
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

static void *publish_state(void *arg) {
    (void)arg;
    while (1) {
        int res = emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        thread_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
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

    // Create a thread to publish state every 5 seconds
    thread_t *pub_thread;
    pub_thread = thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publish_state");
    if (pub_thread == NULL) {
        printf("Failed to create publish thread\n");
        return 1;
    }

    // Main loop to keep the program running
    while (1) {
        thread_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}
