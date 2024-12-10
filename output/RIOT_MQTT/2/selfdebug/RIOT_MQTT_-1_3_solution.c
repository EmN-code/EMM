#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "xtimer.h"
#include "thread.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;

void message_delivery_complete(void *context, MQTTMessage *message) {
    (void)context;
    printf("Message with token value %d delivery confirmed\n", message->id);
}

void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    int rc = MQTTPublish(client, "state/status", &message);
    if (rc != SUCCESS) {
        printf("Error %d from MQTT publish\n", rc);
    }
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) < 0) {
        printf("TCP connect was not successful\n");
        return -1;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32_Client";

    if (MQTTConnect(&client, &connectData) < 0) {
        printf("MQTT connect not successful\n");
        return -1;
    }
    
    printf("MQTT connected\n");

    while (1) {
        publish_state(&client, "work");
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}