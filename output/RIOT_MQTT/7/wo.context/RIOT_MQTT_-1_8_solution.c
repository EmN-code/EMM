#include <stdio.h>
#include <stdlib.h>
#include "msg.h"
#include "mqtt/MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "ESP32_Client"
#define TOPIC "device/state"
#define INTERVAL 5

static MQTTClient client;
static Network network;

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    printf("MQTT Task Started\n");

    unsigned char sendbuf[80], readbuf[80];
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    char host[] = SERVER_IP;
    int rc = ConnectNetwork(&network, host, SERVER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network, return code %d\n", rc);
        return rc;
    }

    rc = MQTTStartTask(&client);
    if (rc != SUCCESS) {
        printf("Failed to start MQTT task, return code %d\n", rc);
        return rc;
    }

    MQTTPublish(&client, "init", "init state", QOS0, 0);
    printf("MQTT Task Started\n");

    while (1) {
        publish_state("work");
        xtimer_sleep(INTERVAL);
        publish_state("not work");
        xtimer_sleep(INTERVAL);
    }

    return 0;
}