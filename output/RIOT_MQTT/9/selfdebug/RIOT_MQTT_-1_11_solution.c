#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "MQTTDeserializePublish.h"
#include "MQTTPacket.h"
#include "MQTTPublish.h"
#include "MQTTSerializePublish.h"
#include "MQTTSubscribe.h"
#include "MQTTUnsubscribe.h"
#include "MQTTYield.h"
#include "random.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "ESP32_Client"
#define TOPIC "state"
#define PAYLOAD_WORKING "work"
#define PAYLOAD_NOT_WORKING "not work"
#define KEEP_ALIVE_INTERVAL 60
#define PUBLISH_INTERVAL 5

static char sendbuf[2048];
static char readbuf[2048];

int main(void) {
    MQTTClient client;
    Network network;
    int rc;
    unsigned char msg_buf[100];
    unsigned char msg_readbuf[100];

    // Initialize the network
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, KEEP_ALIVE_INTERVAL * 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Options for the MQTT connection
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENT_ID;
    data.username.cstring = NULL;
    data.password.cstring = NULL;
    data.keepAliveInterval = KEEP_ALIVE_INTERVAL;
    data.cleansession = 1;

    // Connect the client
    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("MQTT connect failed: %d\n", rc);
        return rc;
    }

    // Publish looping
    while (1) {
        MQTTMessage message;
        memset(&message, 0, sizeof(message));
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (state_is_working()) ? PAYLOAD_WORKING : PAYLOAD_NOT_WORKING;
        message.payloadlen = strlen((const char*) message.payload);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("MQTT publish failed: %d\n", rc);
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}

/**
 * Dummy function to simulate state checking. In a real application, this
 * should be replaced with actual device state logic.
 */
int state_is_working(void) {
    // Simulate state with random number for demonstration (real state logic needed)
    return (random_uint32() % 2 == 0);
}