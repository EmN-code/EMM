// Description: This program connects an ESP32 device to an MQTT-SN broker using the emcute library. It publishes the device's working state every 5 seconds using RIOT OS. The included improvements address the error messages, ensuring the 'inet_pton' function is declared and the 'subscription_topic' variable is used.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC_NAME "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t subscription_topic = { .name = PUB_TOPIC_NAME, .id = 0 }; // Ensure the variable is used

int main(void) {
    sock_udp_ep_t gateway = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gateway.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to the MQTT-SN broker.\n");
        return 1;
    }

    printf("Connected to the broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    // Register the topic
    if (emcute_reg(&subscription_topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic.\n");
        return 1;
    }

    char *states[2] = {"work", "not work"};
    int state_index = 0;

    while (1) {
        state_index = !state_index; // Toggle between work and not work

        if (emcute_pub(&subscription_topic, states[state_index],
                       strlen(states[state_index]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: Unable to publish the message.\n");
        } else {
            printf("Published state: %s\n", states[state_index]);
        }

        xtimer_sleep(PUB_INTERVAL / US_PER_SEC); // Wait for 5 seconds
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}