#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"  // Ensure this is the last header file

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/status"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static void publish_message(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);
    int rc = MQTTPublish(client, MQTT_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Error: Unable to publish message, return code %d\n", rc);
    }
    else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc = 0;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Error: Unable to connect to network, return code %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 4;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error: Unable to connect to MQTT broker, return code %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_message(&client);
        xtimer_usleep(MQTT_INTERVAL);
    }

    return 0;
}