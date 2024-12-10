#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "kernel_types.h"
#include "xtimer.h"
#include "Random.h"
#include "hwtimer_macros.h"
#include "periph_conf.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_example"
#define TOPIC "device/state"
#define DELAY (5U * 1000 * 1000) // 5 seconds in microseconds

static void publish_state(MQTTClient* client, int state) {
    MQTTMessage message;
    char payload[20];
    snprintf(payload, sizeof(payload), "State: %s", (state ? "work" : "not work"));
    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS1;
    message.retained = 0;
    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[128], readbuf[128];
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    if ((rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT)) != SUCCESS) {
        printf("Failed to connect to network, return code: %d\n", rc);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    
    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT broker, return code: %d\n", rc);
        return -1;
    }

    printf("Successfully connected to broker at %s:%d.\n", BROKER_IP, BROKER_PORT);

    int state = 1; // start with "work"

    while (1) {
        publish_state(&client, state);
        state = !state; // toggle state between "work" and "not work"
        xtimer_usleep(DELAY);
    }

    return 0;
}