#include <xtimer.h>                 // For using timers
#include <string.h>                 // For string manipulations
#include <stdio.h>                  // For standard input/output
#include <stdlib.h>                 // For standard library usage
#include <net/sock/udp.h>           // For network communication
#include <net/sock/tcp.h>           // For TCP socket communication
#include "MQTTClient.h"            // For MQTT client functionalities

#define CONFIG_GCOAP_DTLS_HANDSHAKE_TIMEOUT_MSEC (3000)  // Some necessary configuration or define taken from RIOT setup.

// MQTT broker details
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "work_status"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5U * US_PER_SEC)   // 5 seconds in microseconds
#define MQTT_PAYLOAD_ON "work"
#define MQTT_PAYLOAD_OFF "not work"

static char mqtt_sendbuf[MQTT_SENDBUFFER_SIZE], mqtt_readbuf[MQTT_READBUFFER_SIZE];
static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    char buf[100];
    
    sprintf(buf, "{"state": "%s"}", MQTT_PAYLOAD_ON);

    message.qos = QOS0;
    message.retained = 0;
    message.payload = buf;
    message.payloadlen = strlen(buf);

    // Publish the message
    MQTTPublish(&client, MQTT_TOPIC, &message);

    puts("Message published.");
}

int main(void) {
    // Network and MQTT setup
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    // Set MQTT options
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT-Client";

    // Connect to the MQTT broker
    if (ConnectNetwork(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Could not connect to the broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return -1;
    }

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("MQTT connection established\n");

    // Loop to publish every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(MQTT_INTERVAL / US_PER_SEC);
    }

    return 0;
}