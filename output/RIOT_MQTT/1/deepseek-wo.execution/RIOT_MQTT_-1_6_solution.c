
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <mutex.h>
#include <thread.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

typedef struct {
    int socket;
    struct sockaddr_in server;
} Network;

void NetworkInit(Network *n) {
    n->socket = -1;
    n->server.sin_family = AF_INET;
    n->server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &n->server.sin_addr);
}

int NetworkConnect(Network *n) {
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        return -1;
    }
    if (connect(n->socket, (struct sockaddr *)&n->server, sizeof(n->server)) < 0) {
        return -1;
    }
    return 0;
}

int NetworkRead(Network *n, unsigned char *buffer, int len, int timeout) {
    return recv(n->socket, buffer, len, 0);
}

int NetworkWrite(Network *n, unsigned char *buffer, int len) {
    return send(n->socket, buffer, len, 0);
}

void NetworkDisconnect(Network *n) {
    if (n->socket != -1) {
        close(n->socket);
        n->socket = -1;
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char sendbuf[256];
    char readbuf[256];

    NetworkInit(&network);
    if (NetworkConnect(&network) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return 1;
    }

    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    return 0;
}
