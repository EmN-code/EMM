
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "MQTTClient.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000

typedef struct {
    int sock;
} Network;

void NetworkInit(Network *n) {
    n->sock = -1;
}

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    struct sockaddr_in server_addr;
    n->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (n->sock < 0) {
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    inet_pton(AF_INET, address_ip, &server_addr.sin_addr);

    if (connect(n->sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(n->sock);
        n->sock = -1;
        return -1;
    }

    return 0;
}

void NetworkDisconnect(Network *n) {
    if (n->sock != -1) {
        close(n->sock);
        n->sock = -1;
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char sendbuf[200], readbuf[200];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    connectData.keepAliveInterval = MQTT_KEEPALIVE;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return -1;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
