// Import necessary headers in descending alphabetical order
#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

// Definitions
#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "my_mqtt_client"
#define TOPIC_NAME "work_status"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // Publish every 5 seconds

// Function to connect to MQTT-SN broker
int connect_to_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };

    inet_pton(AF_INET6, BROKER_ADDRESS, &gw.addr.ipv6);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker (%d)\n", res);
        return res;
    }
    printf("Connected to broker successfully\n");
    return EMCUTE_OK;
}

// Function to publish to the broker
void publish_state(void) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = TOPIC_NAME;

    const char *status_msg = "work"; // The status message to publish
    int res = emcute_pub(&t, status_msg, strlen(status_msg), flags);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to publish data (%d)\n", res);
    } else {
        printf("Published: %s\n", status_msg);
    }
}

// Main function
int main(void) {
    printf("Starting MQTT-SN publisher example\n");
    
    if (connect_to_broker() != EMCUTE_OK) {
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}