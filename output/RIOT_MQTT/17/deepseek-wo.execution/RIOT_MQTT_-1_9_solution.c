
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_tcp_t sock;

int mqttread(struct Network* network, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqttwrite(struct Network* network, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&sock, buffer, len, SOCK_NO_TIMEOUT);
}

int main(void)
{
    struct sock_tcp_ep remote;
    struct Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    int rc;

    remote.family = AF_INET;
    remote.port = HTONS(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error creating socket");
        return 1;
    }

    network.mqttread = mqttread;
    network.mqttwrite = mqttwrite;

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT-MQTT-Client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return 1;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish, return code %d\n", rc);
        }

        xtimer_sleep(5);
    }

    return 0;
}
