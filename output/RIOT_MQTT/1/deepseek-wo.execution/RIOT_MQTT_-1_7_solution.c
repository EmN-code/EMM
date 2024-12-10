
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <xtimer.h>
#include <mutex.h>
#include <thread.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static void network_init(Network *n) {
    n->socket = -1;
}

static int network_connect(Network *n, const char *address, int port) {
    struct sockaddr_in server_addr;
    n->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (n->socket < 0) {
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    if (connect(n->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(n->socket);
        return -1;
    }

    return 0;
}

static int network_read(Network *n, unsigned char *buffer, int len, int timeout) {
    fd_set readfds;
    struct timeval tv;
    int rc;

    FD_ZERO(&readfds);
    FD_SET(n->socket, &readfds);

    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    rc = select(n->socket + 1, &readfds, NULL, NULL, &tv);
    if (rc > 0) {
        return recv(n->socket, buffer, len, 0);
    } else {
        return -1;
    }
}

static int network_write(Network *n, unsigned char *buffer, int len, int timeout) {
    fd_set writefds;
    struct timeval tv;
    int rc;

    FD_ZERO(&writefds);
    FD_SET(n->socket, &writefds);

    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    rc = select(n->socket + 1, NULL, &writefds, NULL, &tv);
    if (rc > 0) {
        return send(n->socket, buffer, len, 0);
    } else {
        return -1;
    }
}

static void network_disconnect(Network *n) {
    if (n->socket != -1) {
        close(n->socket);
        n->socket = -1;
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    network_init(&network);
    if (network_connect(&network, SERVER_IP, SERVER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        network_disconnect(&network);
        return -1;
    }

    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    network_disconnect(&network);
    return 0;
}
