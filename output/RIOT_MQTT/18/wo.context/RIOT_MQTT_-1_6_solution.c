#include <string.h>
#include <stdio.h>
#include <stdlib.h>  
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32_Client"
#define MQTT_TOPIC "esp32/state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define CONNECTION_TIMEOUT 1000

Network network;
MQTTClient client;
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
int rc;

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*) state;
    message.payloadlen = strlen(state);
    if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != SUCCESS) {
        printf("Error publishing: %d\n", rc);
    }
}

int main(void) {
    char sendbuf[100], readbuf[100];
    NetworkInit(&network);
    MQTTClientInit(&client, &network, CONNECTION_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Error connecting to network: %d\n", rc);
        return -1;
    }
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state(MQTT_PAYLOAD_WORK);
        printf("Published: %s\n", MQTT_PAYLOAD_WORK);
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    return 0;
}
