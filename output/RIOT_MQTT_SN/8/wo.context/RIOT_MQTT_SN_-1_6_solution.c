#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include "emcute.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC  "state"

static emcute_topic_t topic;
static char mqtt_state[10];

void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    printf("State published successfully.\n");
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .addr = { 0 }, .port = SERVER_PORT };
    int res = inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);
    if (res <= 0) {
        printf("Error converting IP address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
        printf("Connected to MQTT-SN broker at %s\n", SERVER_IP);
    }
    else {
        printf("Could not connect to broker\n");
        return 1;
    }

    topic.name = MQTT_TOPIC;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        strcpy(mqtt_state, "work");

        if (emcute_pub(&topic, mqtt_state, strlen(mqtt_state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        else {
            printf("Data published: %s\n", mqtt_state);
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}