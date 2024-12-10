
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void network_init(Network *n)
{
    int res = NetworkConnect(n, SERVER_IP, SERVER_PORT);
    if (res != 0) {
        printf("Failed to connect to network\n");
        return;
    }
}

static void mqtt_init(MQTTClient *client, Network *n)
{
    unsigned char sendbuf[200], readbuf[200];
    MQTTClientInit(client, n, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    int rc = MQTTConnect(client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
}

static void publish_state(MQTTClient *client)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    }
}

int main(void)
{
    Network network;
    MQTTClient client;

    network_init(&network);
    mqtt_init(&client, &network);

    while (1) {
        publish_state(&client);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
