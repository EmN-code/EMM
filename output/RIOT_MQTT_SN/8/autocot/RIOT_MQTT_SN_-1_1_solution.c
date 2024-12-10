#include <xtimer.h>  // For timing functions and delays
#include <stdio.h>   // For input/output functions
#include <string.h>  // For string handling functions
#include "net/emcute.h"  // For emcute (MQTT-SN) related functions and types

// Define constants
#define EMCUTE_PORT     1883
#define EMCUTE_ID       "esp32"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP       "47.102.103.1"
#define TOPIC_NAME      "device/state"
#define PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

// Declare global variables
static emcute_topic_t my_topic;
static char mqtt_buf[128];

// Function to publish a message to a topic
void publish_state(const char *state) {
    // Prepare the message
    strncpy(mqtt_buf, state, sizeof(mqtt_buf));
    mqtt_buf[sizeof(mqtt_buf) - 1] = '\0';  // Ensure null-termination

    // Create the emcute topic
    my_topic.name = TOPIC_NAME;
    my_topic.id = 0;  // Topic id

    // Publish the message
    if (emcute_pub(&my_topic, mqtt_buf, strlen(state), EMCUTE_QOS_0)) {
        puts("[Error] unable to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    // Declare and initialize variables
    emcute_device_t res;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    // Convert IP address
    int ret = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (ret != 1) {
        puts("Invalid broker address");
        return 1;
    }

    // Initialize the emcute
    res = emcute_setup(EMCUTE_ID, EMCUTE_PRIO, gw);
    if (res != 0) {
        puts("Error initializing emcute");
        return 1;
    }

    puts("Connection established");

    // Main loop
    while (1) {
        publish_state("work");  // Publish the state
        xtimer_usleep(PUBLISH_INTERVAL);  // Wait 5 seconds
        publish_state("not work");  // Publish the state
        xtimer_usleep(PUBLISH_INTERVAL);  // Wait 5 seconds
    }

    return 0;
}