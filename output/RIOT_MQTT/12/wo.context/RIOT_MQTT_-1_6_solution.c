#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "random.h"
#include "mqtt/MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_mqtt_client"
#define TOPIC "device/state"
#define USERNAME "your_username"  // if required
#define PASSWORD "your_password"  // if required
#define PUBLISH_INTERVAL (5 * US_PER_SEC)  // 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

static void mqtt_publish_state(void) {
    MQTTMessage message;
    char payload[20];

    snprintf(payload, sizeof(payload), "state: work"); // Example payload

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, TOPIC, &message);
    printf("Published: %s on topic: %s\n", payload, TOPIC);
}

int main(void) {
    printf("MQTT client starting...\n");

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }
    else {
        printf("Connected to network\n");
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    connectData.username.cstring = USERNAME;
    connectData.password.cstring = PASSWORD;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to broker\n");
        return -1;
    }
    else {
        printf("Connected to broker\n");
    }

    while (1) {
        mqtt_publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}