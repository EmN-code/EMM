#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mqtt.h"
#include "net.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"
#define MESSAGE "work"
#define INTERVAL 5

static mqtt_client_t mqtt_client;
static network_t network;

void mqtt_publish_loop(mqtt_client_t *client) {
    int ret;
    mqtt_message_t message;
    while (1) {
        message.payload = MESSAGE;
        message.payloadlen = strlen(MESSAGE);
        message.qos = 1;
        message.retained = 0;

        ret = MQTTPublish(client, TOPIC, &message);
        if (ret != 0) {
            printf("Failed to publish message: %d\n", ret);
        }
        sleep(INTERVAL);
    }
}

int main() {
    int ret;

    NetworkInit(&network);
    ret = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (ret != 0) {
        printf("Failed to connect to broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return EXIT_FAILURE;
    }

    MQTTClientInit(&mqtt_client, &network, 1000, NULL, 0, NULL, 0);

    ret = MQTTConnect(&mqtt_client);
    if (ret != 0) {
        printf("Failed to connect to MQTT broker: %d\n", ret);
        return EXIT_FAILURE;
    }

    mqtt_publish_loop(&mqtt_client);

    MQTTDisconnect(&mqtt_client);
    NetworkDisconnect(&network);

    return EXIT_SUCCESS;
}