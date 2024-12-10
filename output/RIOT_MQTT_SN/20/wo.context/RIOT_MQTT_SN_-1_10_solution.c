#include "msg.h"
#include "emcute.h"
#include "timex.h"
#include "ztimer.h"
#include <stdio.h>
#include <string.h>

#define MQTT_TOPIC "state"
#define MQTT_MSG_WORK "work"
#define MQTT_MSG_NOT_WORK "not work"
#define INTERVAL (5U)

// Declare the topic object
static emcute_topic_t topic;

// Declare and initialize the socket address for the broker
static sock_udp_ep_t gw = {
    .family = AF_INET,
    .port = 1883
};

int main(void) {
    // Step 1: Print Initialization Message
    printf("Starting MQTT-SN publisher with emcute\n");

    // Step 2: Convert IP address to binary form
    if(inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    // Step 3: Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN gateway\n");
        return 1;
    }

    // Step 4: Register the topic
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        emcute_discon();
        return 1;
    }

    // Step 5: Main loop, publish state every 5 seconds
    while (1) {
        const char* message = (ztimer_now(ZTIMER_MSEC) % 10000 < 5000) ? MQTT_MSG_WORK : MQTT_MSG_NOT_WORK;
        printf("Publishing message: %s\n", message);

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        // Delay for the specified interval
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}