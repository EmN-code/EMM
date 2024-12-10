#include "net/emcute.h"
#include "xtimer.h"
#include <stdio.h>

// Define the topic and message to publish
#define STATE_TOPIC_NAME "state"
#define STATE_TOPIC_ID   1
#define STATE_WORK_MSG   "work"
#define STATE_NOT_WORK_MSG "not_work"

static emcute_topic_t topic;

// Function to publish the state message
void publish_state_message(const char *message) {
    // Publish the message to the MQTT-SN broker
    int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish state message\n");
    }
}

int main(void) {
    // Initialize the emcute library and network
    emcute_con(NULL, true, NULL, NULL, 0, 0);

    // Register the topic
    topic.name = STATE_TOPIC_NAME;
    topic.id = STATE_TOPIC_ID;
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Create a loop to publish the state message every five seconds
    while (1) {
        publish_state_message(STATE_WORK_MSG);
        xtimer_sleep(5);
    }

    return 0;
}