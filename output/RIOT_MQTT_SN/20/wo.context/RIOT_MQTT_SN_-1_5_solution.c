#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * ZTIMER_MSEC_SEC)

static void *_stack[THREAD_STACKSIZE_MAIN]; // Fixed: properly defined and used
static char _publish_msg[32];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    // Handle incoming publish packets here
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    emcute_sub_t subscription;
    emcute_topic_t topic;
    sock_udp_ep_t gateway = { .family = AF_INET, .port = BROKER_PORT };

    inet_pton(AF_INET, BROKER_ADDRESS, &gateway.addr.ipv4);

    // Initialize emcute
    emcute_init(_stack, THREAD_STACKSIZE_MAIN - 1, EMCUTE_PRIO); // Fixed: Correct usage of EMCUTE_PRIO

    // Connect to the broker
    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("ERROR: Unable to connect to %s:%d\n", BROKER_ADDRESS, BROKER_PORT);
        return 1;
    }

    // Configure topic for publishing
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("ERROR: Registration failed\n");
        return 1;
    }

    while (1) {
        snprintf(_publish_msg, sizeof(_publish_msg), "work");  // Example message
        emcute_pub(&topic, _publish_msg, strlen(_publish_msg), EMCUTE_QOS_0);

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    // Disconnect before shutting down
    emcute_discon();

    return 0;
}