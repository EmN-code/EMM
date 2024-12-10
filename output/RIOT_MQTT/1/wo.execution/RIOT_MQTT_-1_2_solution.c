#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    char payload[20];
    snprintf(payload, sizeof(payload), "work"); // either "work" or "not work"

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, MQTT_TOPIC, &message);
    printf("Published: %s\n", payload);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 10000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    printf("Connected to broker\n");

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}