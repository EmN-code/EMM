#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "xtimer.h"
#include "mqtt.h"
#include "MQTTClient.h"  // Ensure this is the last header file included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32client"
#define MQTT_TOPIC "device/state"
#define MQTT_MSG "Work"
#define MQTT_KEEPALIVE 60  // Keep alive interval in seconds
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static char mqtt_sendbuf[128];
static char mqtt_readbuf[128];

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    NetworkInit(&network);
    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to network, error code %d\n", rc);
        return EXIT_FAILURE;
    }

    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEPALIVE;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to the broker, error code %d\n", rc);
        return EXIT_FAILURE;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)MQTT_MSG;
        message.payloadlen = strlen(MQTT_MSG);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("Failed to publish message, error code %d\n", rc);
        } else {
            printf("Successfully published message: %s\n", MQTT_MSG);
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}