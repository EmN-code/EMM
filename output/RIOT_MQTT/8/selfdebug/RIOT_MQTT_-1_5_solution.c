#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"
#include "thread.h"
#include "net/sock/udp.h"

#define MQTT_SENDBUFFER_SIZE 128
#define MQTT_READBUFFER_SIZE 128

static MQTTClient client;
static Network network;
static unsigned char mqtt_readbuf[MQTT_READBUFFER_SIZE];
static unsigned char mqtt_sendbuf[MQTT_SENDBUFFER_SIZE];

static int mqtt_connect(Network *network, MQTTClient *client) {
    int rc = NetworkConnect(network, "47.102.103.1", 1883);
    if (rc != SUCCESS) {
        printf("Failed to connect to the network\n");
        return rc;
    }
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "RIOT-Client";
    return MQTTConnect(client, &connect_data);
}

static void publish_state(MQTTClient *client, const char *topic, const char *msg) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)msg;
    message.payloadlen = strlen(msg);
    MQTTPublish(client, topic, &message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, MQTT_SENDBUFFER_SIZE, mqtt_readbuf, MQTT_READBUFFER_SIZE);

    if (mqtt_connect(&network, &client) != SUCCESS) {
        printf("Cannot connect to MQTT broker\n");
        return -1;
    }

    const char *topic = "esp32/state";
    const char *state_work = "work";
    const char *state_not_work = "not work";

    while (1) {
        publish_state(&client, topic, state_work);
        xtimer_sleep(5);
        publish_state(&client, topic, state_not_work);
        xtimer_sleep(5);
    }

    return 0;
}