#include <thread.h>  // Ensure threading related functions and macros are available
#include <stdint.h>   // Standard Integer types
#include <stdio.h>    // Standard I/O functions
#include <string.h>   // String handling functions
#include <net/sock/udp.h> // Required for UDP networking
#include <xtimer.h>   // Timer functions for periodic execution
#include <msg.h>      // Messaging functions for synchronizing threads
#include "emcute.h"  // Include emcute header for MQTT-SN functions

#define EMCUTE_BUFSIZE (128U)
#define TOPIC_MAXLEN   (32U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

static void *mqtt_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    /* The remote address where the broker resides */
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("ERROR: unable to parse the provided address");
        return NULL;
    }

    /* Connect to the MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to gateway");
        return NULL;
    }

    /* Register the topic to publish to */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return NULL;
    }

    while (1) {
        const char *state_msg = "work";  // The message to be published
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: unable to publish data");
        }
        xtimer_sleep(5);  // Sleep for 5 seconds before sending the next message
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt");

    return 0;
}