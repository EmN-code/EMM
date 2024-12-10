#include <ztimer.h>  // include ztimer before string.h
t include "net/emcute.h"
#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h" // Order: ztimer, string, stdio, sock/udp

enum { EMCUTE_PORT = 1883, PUB_INTERVAL = 5 }; // MQTT port and publish interval

static char mqtt_topic[] = "device/state";
static char mqtt_msg[] = "work";
static emcute_topic_t topic_1;
static sock_udp_ep_t gw;

// Callback for MQTT-SN message receive
static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    // Handle received message if necessary
    printf("Received publication from topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    // Address of the MQTT-SN broker
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    gw.port = EMCUTE_PORT;

    // Connect to the MQTT gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    // Register a topic
    topic_1.name = mqtt_topic;
    if (emcute_reg(&topic_1) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        // Publish the message
        if (emcute_pub(&topic_1, mqtt_msg, strlen(mqtt_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish to topic\n");
        } else {
            printf("Published %s to topic '%s'\n", mqtt_msg, topic_1.name);
        }

        // Sleep for a set interval
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);
    }

    // Disconnect (optional, this part won't be actually reached)
    if (emcute_discon() != EMCUTE_OK) {
        printf("Error: unable to disconnect from gateway\n");
    }

    return 0;
}