// Description
// The following code implements an MQTT-SN client on ESP32 using the RIOT operating system. It connects to a broker at 47.102.103.1 on port 1883 and publishes the device state ('work' or 'not work') every five seconds. The EMCUTE library manages MQTT-SN communication.

// Import required libraries
#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/emcute.h"

#define EMCUTE_PORT      (1883U)
#define STATE_TOPIC      "state_topic"
#define MQTT_BROKER_IP   "47.102.103.1"

static emcute_topic_t state_topic;

// Function to connect to the MQTT broker
static int connect_to_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (res == 0) {
        puts("Error: Invalid address provided");
        return 1;
    }
    else if (res < 0) {
        puts("Error: Could not convert address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to broker");
        return 1;
    }

    return 0;
}

static int publish_state(const char *state) {
    emcute_topic_t t;
    t.name = STATE_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Failed to publish message");
        return 1;
    }

    return 0;
}

int main(void) {
    // Connect to the broker
    if (connect_to_broker() != 0) {
        return 1;
    }

    while (1) {
        // Publish 'work' state
        if (publish_state("work") != 0) {
            puts("Error publishing work state");
        }

        // Wait 5 seconds
        xtimer_sleep(5);

        // Publish 'not work' state
        if (publish_state("not work") != 0) {
            puts("Error publishing not work state");
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}