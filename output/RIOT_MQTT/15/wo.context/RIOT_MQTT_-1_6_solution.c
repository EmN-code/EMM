#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <msg.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_BUFFER_SIZE 256
#define MQTT_PUBLISH_INTERVAL (5LU * US_PER_SEC)

static char mqtt_client_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t mqtt_msg_queue[8];

static int publish_state(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage pub_message;
    pub_message.qos = QOS0;
    pub_message.retained = 0;
    pub_message.dup = 0;
    pub_message.payload = (void *)message;
    pub_message.payloadlen = strlen(message);

    return MQTTPublish(client, topic, &pub_message);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    char client_id[] = "riot_client";
    unsigned char sendbuf[MQTT_BUFFER_SIZE], readbuf[MQTT_BUFFER_SIZE];
    int rc;

    printf("Starting MQTT example\n");

    NetworkInit(&network);
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Error connecting to network. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_BUFFER_SIZE, readbuf, MQTT_BUFFER_SIZE);
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = client_id;

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != SUCCESS) {
            printf("Failed to connect to broker. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT broker\n");
            break;
        }
    }

    char *topic = "device/state";
    char *state_working = "work";
    char *state_not_working = "not work";

    while(1) {
        rc = publish_state(&client, topic, state_working);
        if (rc != SUCCESS) {
            printf("Failed to publish state\n");
        } else {
            printf("State published: work\n");
        }

        xtimer_sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}