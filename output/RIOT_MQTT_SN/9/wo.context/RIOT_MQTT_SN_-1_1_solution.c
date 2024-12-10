#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include <stdio.h>
#include <xtimer.h>

// Definitions for the MQTT-SN
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"
#define PUB_INTERVAL (5U * US_PER_SEC) // 5 seconds

// Define the MQTT-SN topic
static emcute_topic_t topic;

// Stack for the publisher thread
static char stack[THREAD_STACKSIZE_MAIN];

// Function to publish state
static void *publish_state(void *arg) {
    (void)arg;

    while (1) {
        // Publish the "work" state
        emcute_pub(&topic, STATE_WORK, strlen(STATE_WORK), EMCUTE_QOS_0);
        printf("Published: %s\n", STATE_WORK);

        // Sleep for the defined interval
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    // Configure the network
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT, .addr.ipv4 = { .u32 = inet_addr(BROKER_IP) } };

    // Initialize the emcute
    emcute_run(THREAD_PRIORITY_MAIN, stack, sizeof(stack), "mqttsn");
    emcute_con(&gw, true, "uniq_client_id");

    // Configure the topic
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    // Create the publisher thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_state, NULL, "publisher");

    return 0;
}