#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>

#include "net/emcute.h"

#define EMCUTE_PORT     1883
#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_TOPIC_NAME "device/state"
#define INTERVAL        (5 * US_PER_SEC) // 5 seconds interval

typedef struct {
    emcute_topic_t topic;
    char *state;
} mqtt_sn_msg_t;

static emcute_topic_t _mqtt_topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Data received on topic %s: %.*s\n", topic->name, (int)len, (char *)data);
}

void setup_mqtt_sn_client(void) {
    char *state_msg = "work";
    while (1) {
        if (emcute_connect(MQTT_BROKER_IP, EMCUTE_PORT) == 0) {
            printf("Successfully connected to MQTTSN broker\n");
            break;
        } else {
            printf("Failed to connect to MQTTSN broker, retrying...\n");
            xtimer_sleep(1);
        }
    }

    _mqtt_topic.name = MQTT_TOPIC_NAME;
    _mqtt_topic.id = 0; // Topic ID assigned by broker

    while (1) {
        if (emcute_pub(&_mqtt_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) == 0) {
            printf("Message published: %s\n", state_msg);
        } else {
            printf("Failed to publish message\n");
        }

        // Alternate the state
        state_msg = (strcmp(state_msg, "work") == 0) ? "not work" : "work";

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
}

int main(void) {
    emcute_setup(on_pub);
    setup_mqtt_sn_client();
    return 0;
}