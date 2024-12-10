#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqttsn.h>
#include "MQTTClient.h"

#define MQTT_BROKER_ADDR  "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define PUBLISH_INTERVAL  (5U * US_PER_SEC) // 5 seconds

static char mqtt_send_buffer[128];
static char mqtt_read_buffer[128];

void messageArrived(MessageData* data) {
    printf("Message arrived on topic: %.*s: %.*s\n", 
           data->topicName->lenstring.len, data->topicName->lenstring.data, 
           data->message->payloadlen, data->message->payload);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    int rc;

    char* topic = "sensor/status";
    char* payload = "work";

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_ADDR, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network Connection failed: %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, mqtt_send_buffer, sizeof(mqtt_send_buffer), mqtt_read_buffer, sizeof(mqtt_read_buffer));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT Connection failed: %d\n", rc);
        return -1;
    }

    rc = MQTTSubscribe(&client, topic, QOS0, messageArrived);
    if (rc != SUCCESS) {
        printf("MQTT Subscribe failed: %d\n", rc);
        return -1;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    while(1) {
        rc = MQTTPublish(&client, topic, &message);
        if (rc != SUCCESS) {
            printf("MQTT Publish failed: %d\n", rc);
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&client);

    return 0;
}