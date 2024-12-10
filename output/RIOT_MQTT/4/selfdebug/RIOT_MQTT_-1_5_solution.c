#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"


#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static Network network;
static MQTTClient client;
static unsigned char sendbuf[80], readbuf[80];

static void publish_state(void) {
    MQTTMessage message;
    char* payload = "state: work";
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);
    MQTTPublish(&client, "device/state", &message);
    printf("Published: %s\n", payload);
}

int main(void) {
    int rc;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Network connection failed\n");
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }
    printf("MQTT Connected\n");

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}