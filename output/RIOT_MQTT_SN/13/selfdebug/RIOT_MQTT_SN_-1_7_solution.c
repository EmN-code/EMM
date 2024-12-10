// This program connects to an MQTT-SN broker using the emcute library on an ESP32.
// It publishes the state ('work' or 'not work') every five seconds.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           (0)
#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

// MQTT-SN state messages
#define WORK_STATE_MSG      "work"
#define NOT_WORK_STATE_MSG  "not work"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];

static const char *state_messages[] = {WORK_STATE_MSG, NOT_WORK_STATE_MSG};

static emcute_topic_t t_state;
static char *mqtt_broker_ip = "47.102.103.1";
static uint16_t mqtt_broker_port = 1883;

static void pub_state(void) {
    static int state = 0;
    state = !state;  // Toggle state
    const char *msg = state_messages[state];

    // Set the MQTT-SN topic
    t_state.name = "device/state";
    t_state.id = 1;  // Assuming the topic is registered and has ID 1

    // Publish the message
    int res = emcute_pub(&t_state, msg, strlen(msg), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data [%d]\n", res);
    }
}

static void *emcute_thread(void *arg) {
    while (1) {
        pub_state();
        xtimer_sleep(5);  // Wait for 5 seconds
    }

    return NULL;
}

int main(void) {
    // Initialize the MQTT-SN connection
    emcute_con(mqtt_broker_ip, mqtt_broker_port, NULL);

    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}