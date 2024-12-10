#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>  // for sleeping between publishes
#include <net/sock/udp.h>  // socket programming
#include <net/emcute.h>  // for MQTT functionality definitions, if using emcute
#include "MQTTClient.h"  // ensure this is last if being used

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define PUBLISH_INTERVAL 5 // in seconds

static char *mqtt_topic = "device/state";

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");
    MQTTPublish(client, mqtt_topic, &message);

    message.payload = (void *)"not work";
    message.payloadlen = strlen("not work");
    MQTTPublish(client, mqtt_topic, &message);
}

int main(void) {
    int rc = 0;
    unsigned char sendbuf[80], readbuf[80];
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to broker\n");
        return rc;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return rc;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return rc;
}