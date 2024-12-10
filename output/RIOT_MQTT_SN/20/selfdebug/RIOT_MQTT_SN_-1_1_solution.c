#include <stdio.h>
#include <string.h>
#include "MQTTPacket.h"
#include "emcute.h"

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static emcute_cnt_t emcute;

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    emcute_topic_t topic = {
        .name = "device/state",
        .id = 0
    };

    int result = emcute_pub(&topic, &message);
    if (result != 0) {
        printf("Failed to publish state: %d\n", result);
    }
}

int main(void) {
    printf("Starting MQTT-SN client\n");
    emcute.msg_type = "MQTT-SN";
    emcute.broker.ipv4 = MQTT_SN_BROKER_IP;
    emcute.broker.port = MQTT_SN_BROKER_PORT;

    if (emcute_connect(&emcute) != MQ_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }

    while (1) {
        publish_state("work");
        sleep(PUBLISH_INTERVAL);

        publish_state("not work");
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}