#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xtimer.h>
#include "net/sock.h"
#include "net/ipv6/addr.h"
#include "MQTTClient.h"

#define MQTT_SERVER_HOST   "47.102.103.1"
#define MQTT_SERVER_PORT   1883
#define MQTT_CLIENT_ID     "riot-esp32-client"
#define MQTT_KEEP_ALIVE    60
#define TOPIC              "device/state"
#define QOS                QOS0

static MQTTClient client;
static Network network;

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);
    int result = MQTTPublish(&client, TOPIC, &message);
    if (result != SUCCESS) {
        printf("Failed to publish message to topic %s. Error: %d\n", TOPIC, result);
    }
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_SERVER_HOST, MQTT_SERVER_PORT);
    if (rc != 0) {
        printf("Network connection failed\n");
        return rc;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEP_ALIVE;
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connection failed\n");
        return rc;
    }

    while (1) {
        publish_state("work");
        xtimer_sleep(5);
        publish_state("not work");
        xtimer_sleep(5);
    }
    return 0;
}