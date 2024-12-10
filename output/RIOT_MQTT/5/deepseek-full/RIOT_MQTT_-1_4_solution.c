
#include <stddef.h>
#include <timex.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <stdlib.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <ztimer.h>
#include <MQTTClient.h>
#include <msg.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

void messageArrived(MessageData* data) {
    (void)data;
}

int main(void) {
    unsigned char sendbuf[256];
    unsigned char readbuf[256];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
