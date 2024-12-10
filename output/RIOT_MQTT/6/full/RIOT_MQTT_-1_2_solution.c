#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/protnum.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define DEVICE_ID "esp32"
#define QOS 1
#define TIMEOUT 10000L

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    char payload[20];

    sprintf(payload, "state: %s", (rand() % 2) ? "work" : "not work");
    message.qos = QOS;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    int rc;
    while ((rc = MQTTPublish(client, "esp32/state", &message)) != 0) {
        printf("Failed to publish message, return code %d\n", rc);
        xtimer_sleep(1);
    }
    printf("Message published: %s\n", payload);
}

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, 
                   (unsigned char*)malloc(100), 100,
                   (unsigned char*)malloc(100), 100);

    while (1) {
        while ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
            printf("Network connection failed. Return code: %d\n", rc);
            xtimer_sleep(1);
        }

        printf("Network connected\n");

        MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
        connectData.MQTTVersion = 3;
        connectData.clientID.cstring = DEVICE_ID;

        while ((rc = MQTTConnect(&client, &connectData)) != 0) {
            printf("MQTT connection failed. Return code: %d\n", rc);
            NetworkDisconnect(&network);
            xtimer_sleep(1);
        }

        printf("MQTT connected\n");

        while (1) {
            publish_state(&client);
            xtimer_sleep(5);
        }

        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    return 0;
}