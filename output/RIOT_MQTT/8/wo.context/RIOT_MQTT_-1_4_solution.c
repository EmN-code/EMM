#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <MQTTPacket.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"

static const char *work_state = "work";
static const char *not_work_state = "not work";


int main(void) {
    // Network setup
    Network network;
    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error: Could not connect to broker\n");
        return rc;
    }

    // Initialize MQTT Client
    unsigned char sendbuf[80], readbuf[80];
    MQTTClient client;
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error: Failed to connect to the broker\n");
        return rc;
    }

    // Publish state every 5 seconds
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    while (1) {
        message.payload = (void*)work_state;
        message.payloadlen = strlen(work_state);
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Error: Failed to publish message\n");
            break;
        }

        xtimer_sleep(PUBLISH_INTERVAL);
        // Toggle state between 'work' and 'not work'
        const char *temp = work_state;
        work_state = not_work_state;
        not_work_state = temp;
    }

    // Disconnect from the broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}