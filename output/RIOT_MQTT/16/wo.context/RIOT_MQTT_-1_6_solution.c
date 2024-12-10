#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)

static sock_tcp_t sock;
static MQTTClient client;
static Network network;

void mqtt_publish_task(void) {
    MQTTMessage message;
    char payload[20];

    snprintf(payload, sizeof(payload), "work");

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    while (1) {
        if (MQTTPublish(&client, "state", &message) != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: %s\n", payload);
        }
        xtimer_sleep(MQTT_PUBLISH_INTERVAL / US_PER_SEC);
    }
}

void setup_mqtt_client(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
    } else {
        printf("Connected to MQTT broker\n");
    }
}

int main(void) {
    setup_mqtt_client();

    mqtt_publish_task();

    return 0;
}