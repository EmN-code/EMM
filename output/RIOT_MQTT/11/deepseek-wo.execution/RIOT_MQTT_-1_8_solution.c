
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include "MQTTClient.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define SEND_BUF_SIZE 100
#define READ_BUF_SIZE 100

typedef struct {
    int socket;
    struct sockaddr_in addr;
} Network;

void NetworkInit(Network *n) {
    n->socket = -1;
    memset(&n->addr, 0, sizeof(n->addr));
}

int NetworkConnect(Network *n, char *address_ip, int port_number) {
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        return -1;
    }

    n->addr.sin_family = AF_INET;
    n->addr.sin_port = htons(port_number);
    inet_pton(AF_INET, address_ip, &n->addr.sin_addr);

    if (connect(n->socket, (struct sockaddr *)&n->addr, sizeof(n->addr)) < 0) {
        return -1;
    }

    return 0;
}

void NetworkDisconnect(Network *n) {
    if (n->socket >= 0) {
        close(n->socket);
        n->socket = -1;
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[SEND_BUF_SIZE];
    unsigned char readbuf[READ_BUF_SIZE];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEPALIVE;
    connectData.cleansession = 1;

    if (NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return -1;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
