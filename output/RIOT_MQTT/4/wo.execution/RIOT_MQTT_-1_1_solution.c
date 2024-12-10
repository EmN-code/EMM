#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define QOS 1
#define TOPIC "device/state"
#define BROKER_TIMEOUT_MS 30000
#define PUBLISH_INTERVAL_US (5 * US_PER_SEC)

static char sendbuf[100];
static char readbuf[100];

int main(void)
{
    MQTTClient client;
    Network network;
    int rc;
    MQTTMessage message;
    char *state_work = "work";
    char *state_not_work = "not work";
    int toggle = 0;

    // Initialize the network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, BROKER_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Error - NetworkConnect failed: %d\n", rc);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error - MQTTConnect failed: %d\n", rc);
        return -1;
    }

    printf("MQTT client connected to broker %s:%d\n", BROKER_IP, BROKER_PORT);

    while (1) {
        message.qos = QOS;
        message.retained = 0;
        message.payload = (toggle ? state_work : state_not_work);
        message.payloadlen = strlen((char *)message.payload);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Error - MQTTPublish failed: %d\n", rc);
        }
        else {
            printf("Published state: %s\n", (char *)message.payload);
        }

        toggle = !toggle;

        xtimer_usleep(PUBLISH_INTERVAL_US);
    }

    // Disconnect the MQTT client
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}