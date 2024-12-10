#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_riot"
#define PUBLISH_TOPIC "esp32/status"
#define BUFFER_SIZE 128

static mqtt_client_t mqtt_client;
static mqtt_broker_t broker;

void publish_state(void) {
    mqtt_message_t message;
    message.qos = MQTT_QOS_0;
    message.retained = 0;
    message.payload = "work or not work";
    message.payloadlen = strlen(message.payload);

    int ret = mqtt_publish(&mqtt_client, &broker, PUBLISH_TOPIC, &message);
    if (ret != MQTT_OK) {
        printf("Error while publishing: %d\n", ret);
    }
}

int main(void) {
    printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    mqtt_init(&mqtt_client, &broker, CLIENT_ID, BUFFER_SIZE);

    int ret = mqtt_connect(&mqtt_client, &broker, BROKER_IP, BROKER_PORT);
    if (ret != MQTT_OK) {
        printf("Error connecting to broker: %d\n", ret);
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}