#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define EMCUTE_ID           "riot-node"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_PORT         1883
#define BROKER_ADDRESS      "47.102.103.1"
#define TOPIC_NAME          "device/state"
#define INTERVAL            (5U * US_PER_SEC)  // 5 seconds

// Message queue
static msg_t _queue[8];
static char emcute_stack[THREAD_STACKSIZE_MAIN];

// Global variable to track the working state
static bool is_working = true;

// Function to toggle and publish the state
void *publisher_thread(void *arg){
    emcute_topic_t topic;
    unsigned flags = EMCUTE_QOS_0;

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic.\n");
        return NULL;
    }

    while (1) {
        // Toggle between work and not work
        const char *state = is_working ? "work" : "not work";
        is_working = !is_working;

        if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
            printf("Error: unable to publish data.\n");
        }
        else {
            printf("Published state: %s\n", state);
        }

        // Wait for the interval
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(_queue, ARRAY_SIZE(_queue));

    // Setup address
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker.\n");
        return 1;
    }
    printf("Connected to broker %s:%d\n", BROKER_ADDRESS, BROKER_PORT);

    // Start the publisher thread
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  publisher_thread, NULL, "publisher");

    // Main thread sleep
    while (1) {
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}