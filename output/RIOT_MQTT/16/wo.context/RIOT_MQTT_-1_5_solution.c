#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/af.h"
#include "net/protnet.h"
#include "net/sock/udp.h"
#include "mqttclient.h" // Ensure this is the last include

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "esp32/state"
#define BUFFER_SIZE 128
#define SLEEP_INTERVAL 5 // seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[BUFFER_SIZE];
static unsigned char readbuf[BUFFER_SIZE];
static const char *state_message = "work";

void mqtt_publish(MQTTClient *client)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state_message;
    message.payloadlen = strlen(state_message);

    if (MQTTPublish(client, PUB_TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: %s\n", (char *)message.payload);
    }
}

int main(void)
{
    // Network initialization
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    // Connect to the network
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Network connection failed\n");
        return -1;
    }

    // MQTT connection
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connection failed\n");
        return -1;
    }

    printf("MQTT connected\n");

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}