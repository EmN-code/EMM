#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <timex.h>
#include <net/sock/tcp.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

MQTTClient client;
Network network;

void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, "device/state", &message);
}

int client_connect(void) {
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return -1;
    }
    printf("Network connected\n");

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connect failed\n");
        return -1;
    }
    printf("MQTT connected\n");
    return 0;
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (client_connect() != 0) {
        printf("Attempting to reconnect...\n");
        xtimer_sleep(5);
    }

    while (1) {
        char *state = "work";
        publish_state(state);
        printf("Published: %s\n", state);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}