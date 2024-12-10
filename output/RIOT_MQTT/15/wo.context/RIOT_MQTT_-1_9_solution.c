#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "net/emcute.h"
#include "MQTTClient.h"

#define MSG_TYPE_MQTT_PUB       (0xA0)
#define MQTT_BROKER_IP          "47.102.103.1"
#define MQTT_BROKER_PORT        1883
#define TOPIC_NAME              "state"
#define MESSAGE                 "work or not work"
#define PUBLISH_INTERVAL        (5U * US_PER_SEC)  // 5 seconds

#define MQTT_SENDBUF_SIZE       512
#define MQTT_READBUF_SIZE       512

static char sendbuf[MQTT_SENDBUF_SIZE];
static char readbuf[MQTT_READBUF_SIZE];

MQTTClient client;
Network network;

int main(void) {
    int rc;

    // Initialize the network
    NetworkInit(&network);

    // Connect to the MQTT broker
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully!\n");
            break;
        }
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_SENDBUF_SIZE, readbuf, MQTT_READBUF_SIZE);

    // Connect the MQTT client
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";

    while (1) {
        rc = MQTTConnect(&client, &options);
        if (rc != 0) {
            printf("MQTT connection failed, retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connected successfully!\n");
            break;
        }
    }

    // Publish messages every 5 seconds
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = false;
    message.payload = (void *)MESSAGE;
    message.payloadlen = strlen(MESSAGE);

    while (1) {
        rc = MQTTPublish(&client, TOPIC_NAME, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message, retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Message published successfully!\n");
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}