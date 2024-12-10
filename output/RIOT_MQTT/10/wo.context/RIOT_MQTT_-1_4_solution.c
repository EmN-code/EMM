#include <string.h>
#include <stdio.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static const char* message = "work";

static void publish_state(MQTTClient* client) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.dup = 0;
    mqtt_message.payload = (void*)message;
    mqtt_message.payloadlen = strlen(message);
    
    MQTTPublish(client, MQTT_TOPIC, &mqtt_message);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Network connection failed\n");
        return -1;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}