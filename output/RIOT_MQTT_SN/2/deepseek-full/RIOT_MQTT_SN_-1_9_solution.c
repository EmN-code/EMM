
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
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"
#define STATE_MESSAGE_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;
static char state_message[STATE_MESSAGE_LEN] = STATE_MESSAGE;

static void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, state_message, STATE_MESSAGE_LEN, 0);
        if (res != EMCUTE_OK) {
            puts("Failed to publish state");
        }
        thread_sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Failed to connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    // Create a thread to publish state every 5 seconds
    thread_t *pub_thread = thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, publish_state,
                                         NULL, "publish_state");
    if (pub_thread == NULL) {
        puts("Failed to create publish thread");
        return 1;
    }

    return 0;
}
