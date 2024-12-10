#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP       "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define PUB_TOPIC            "state"
#define MQTT_CLIENT_ID       "riot-esp32"
#define MQTT_USERNAME        ""
#define MQTT_PASSWORD        ""
#define UPDATE_INTERVAL      (5U) // seconds

static MQTTClient client;
static Network network;

static unsigned char sendbuf[MQTT_SENDBUF_SIZE], readbuf[MQTT_READBUF_SIZE];

static void mqtt_publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    while (MQTTPublish(&client, PUB_TOPIC, &message) != SUCCESS) {
        printf("Failed to publish the message. Retrying in 1 second...\n");
        xtimer_sleep(1);
    }
    printf("Message published successfully\n");
}

int main(void) {
    int rc;

    // Initialize network
    NetworkInit(&network);

    // Connect to the network
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to network\n");
        break;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.username.cstring = MQTT_USERNAME;
    connectData.password.cstring = MQTT_PASSWORD;

    // Connect to MQTT broker
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != SUCCESS) {
            printf("MQTT connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to MQTT broker\n");
        break;
    }

    // Periodically publish the state
    const char *state = "work";

    while (1) {
        mqtt_publish_state(state);
        xtimer_sleep(UPDATE_INTERVAL);
        state = strcmp(state, "work") == 0 ? "not work" : "work";
    }

    return 0;
}