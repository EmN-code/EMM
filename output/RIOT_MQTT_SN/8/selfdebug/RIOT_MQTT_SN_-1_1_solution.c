#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include "thread.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32-client"
#define PUB_TOPIC_NAME "state"
#define PUB_MSG_WORK "work"
#define PUB_MSG_NOT_WORK "not work"
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char message[20];

static void *publisher_thread(void *arg) {
    (void)arg; // unused
    while(1) {
        // Publish the state message
        strcpy(message, PUB_MSG_WORK);  // or PUB_MSG_NOT_WORK as required
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);

        // Wait for next publish cycle
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    // Network configuration
    static emcute_sub_t subscriptions[1];
    static msg_t queue[8];

    // Initialize the emcute library with a smaller stack size
    emcute_run(CLIENT_ID, queue, sizeof(queue)/sizeof(queue[0]), subscriptions, sizeof(subscriptions)/sizeof(subscriptions[0]));

    // Setup topic
    topic.name = PUB_TOPIC_NAME;
    topic.id = 0;  // dynamic topic ID
    emcute_reg(&topic);

    // Start publisher thread
    char pub_stack[THREAD_STACKSIZE_MAIN];
    thread_create(pub_stack, sizeof(pub_stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    while(1) {
        // Keep the main thread running
        xtimer_sleep(60);
    }
    return 0;
}