
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_publish(MQTTClient *client, const char *topic, const char *payload, int payload_len) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = payload_len;

    MQTTPublish(client, topic, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    NetworkInit(&network);
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&mqtt_client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        mqtt_publish(&mqtt_client, TOPIC, PAYLOAD, PAYLOAD_LEN);
        printf("Published: %s\n", PAYLOAD);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
