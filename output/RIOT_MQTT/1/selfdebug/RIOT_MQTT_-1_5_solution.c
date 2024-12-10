#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <thread.h>
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_PAYLOAD "work or not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);
    
    printf("Publishing message: %s\n", MQTT_PAYLOAD);
    MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
}

int main(void)
{
    printf("MQTT client setup\n");
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectionData = MQTTPacket_connectData_initializer;
    connectionData.MQTTVersion = 3;
    connectionData.clientID.cstring = "RIOT-ESP32-Client";

    printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTConnect(&client, &connectionData);

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}