#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static const char *TOPIC_NAME = "device/state";
static const char *state_work = "work";
static const char *state_not_work = "not work";

void *publish_state(void *arg) {
    (void)arg;
    while (1) {
        // Toggle state
        const char *state = (state[0] == 'w') ? state_not_work : state_work;

        // Publish the state message
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        // Sleep for defined interval
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    // Initialize Emcute
    emcute_run(BROKER_PORT, "device1");

    // Configure the broker endpoint
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        printf("Error: unable to parse broker IP address\n");
        return 1;
    }

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    // Setup the topic to publish
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Create a thread to publish data periodically
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_state, NULL, "publish");

    return 0;
}