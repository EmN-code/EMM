#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/sock/tcp.h"
#include "xtimer.h"
#include "MQTTClient.h"  // This must be the last to ensure previous types are defined

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC)  // 5 seconds
#define MQTT_TOPIC       "device/state"

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void setup_network(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
}

void setup_mqtt(void) {
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "esp32_client";

    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    int rc = MQTTConnect(&mqtt_client, &connect_data);
    if (rc != SUCCESS) {
        printf("Failed to connect to broker, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void publish_state(void) {
    MQTTMessage message;
    char* states[] = {"work", "not work"};
    message.qos = QOS0;
    message.retained = 0;
    message.payload = states[0];  // Change index between 0 and 1 as needed
    message.payloadlen = strlen(states[0]);
    
    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published: %s\n", (char *)message.payload);
    }
}

int main(void) {
    setup_network();
    setup_mqtt();

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    NetworkDisconnect(&network);
    printf("Disconnected from broker.\n");

    return 0;
}