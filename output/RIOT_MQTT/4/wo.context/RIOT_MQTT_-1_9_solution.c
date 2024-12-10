#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdint.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUB_MSG "work or not work"
#define PUB_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static MQTTClient client;
static Network network;

void publish(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = false;
    message.payload = PUB_MSG;
    message.payloadlen = strlen(PUB_MSG);
    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void)
{
    printf("MQTT client application started\n");

    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish();
        MQTTYield(&client, PUB_INTERVAL * 1000);
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}