#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "net/sock/udp.h"
#include "net/emcute.h"

#define MQTT_TOPIC "device/state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883U

static sock_udp_ep_t broker_endpoint;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published to topic %s: %s\n", topic->name, (char *)data);
}

static int connect_to_broker(void) {
    memset(&broker_endpoint, 0, sizeof(broker_endpoint));
    broker_endpoint.family = AF_INET;
    if (inet_pton(AF_INET, BROKER_IP, &broker_endpoint.addr) == 0) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }
    broker_endpoint.port = BROKER_PORT;

    if (emcute_con(&broker_endpoint, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("MQTT-SN: Unable to connect to broker");
        return 1;
    }

    puts("MQTT-SN: Connected to broker");
    return 0;
}

static void publish_state(void) {
    static emcute_topic_t topic;
    topic.name = MQTT_TOPIC;

    int state = time(NULL) % 2; // Simulating state change between 0 and 1
    char *message = state ? "work" : "not work";

    if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish");
    }
    else {
        printf("Published: %s in state %s\n", topic.name, message);
    }
}

int main(void) {
    if (connect_to_broker() != 0) {
        return 1;
    }

    while (1) {
        publish_state();
        sleep(5);
    }

    return 0;
}