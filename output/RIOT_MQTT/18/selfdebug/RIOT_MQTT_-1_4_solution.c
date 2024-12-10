#include "net/sock/tcp.h"
#include "net/sock/udp.h"
#include "net/sock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ztimer.h"
#include "shell.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "esp32/status"
#define PUBLISH_INTERVAL (5U * 1000000U) /* 5 seconds in microseconds */

static int publish_state(Network *network, const char *state) {
    MQTTMessage message;
    memset(&message, 0, sizeof(MQTTMessage));
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    return MQTTPublish(&network->mqtt_client, TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTConnect(&client, &connectData);

    // Main loop: publish state every 5 seconds
    while (1) {
        publish_state(&network, "work");
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}