#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conn_interface.h"
#include "MQTTPacket.h"
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];
static char topic[] = "device/state";

void connect_to_mqtt_broker(void) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = ConnectNetwork(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to network with return code: %d\n", rc);
        exit(EXIT_FAILURE);
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to MQTT broker with return code: %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Connected to MQTT broker!\n");
}

void publish_state(void) {
    int rc;
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work or not work";
    message.payloadlen = strlen("work or not work");

    if ((rc = MQTTPublish(&client, topic, &message)) != 0) {
        printf("Failed to publish message with return code: %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Message published: %s\n", (char *)message.payload);
}

int main(void) {
    connect_to_mqtt_broker();
    while (1) {
        publish_state();
        sleep(MQTT_PUBLISH_INTERVAL);
    }
    return 0;
}