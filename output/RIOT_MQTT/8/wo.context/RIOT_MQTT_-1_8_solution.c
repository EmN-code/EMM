#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock.h>
#include "MQTTClient.h" // must be the last header if used

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32_Client"
#define MQTT_TOPIC "state_topic"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEPALIVE_INTERVAL 10
#define MQTT_QOS 1

static int _publish_message(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = MQTT_QOS;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);

    return MQTTPublish(client, topic, &mqtt_message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error: Could not connect to the broker.\n");
        return rc;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;
    connect_data.keepAliveInterval = MQTT_KEEPALIVE_INTERVAL;
    connect_data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &connect_data)) != SUCCESS) {
        printf("Error: Could not establish a connection with the broker.\n");
        return rc;
    }

    while (1) {
        rc = _publish_message(&client, MQTT_TOPIC, MQTT_PAYLOAD);
        if (rc != SUCCESS) {
            printf("Error: Could not publish message.\n");
        } else {
            printf("Message published successfully.\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}