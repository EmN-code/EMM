
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

typedef struct {
    int socket;
    struct sockaddr_in server_addr;
} Network;

void NetworkInit(Network *n) {
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    n->server_addr.sin_family = AF_INET;
    n->server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &n->server_addr.sin_addr);
}

int NetworkConnect(Network *n) {
    return connect(n->socket, (struct sockaddr *)&n->server_addr, sizeof(n->server_addr));
}

int NetworkSend(Network *n, unsigned char *buf, int len) {
    return send(n->socket, buf, len, 0);
}

int NetworkReceive(Network *n, unsigned char *buf, int len) {
    return recv(n->socket, buf, len, 0);
}

void NetworkDisconnect(Network *n) {
    close(n->socket);
}

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

void *mqtt_thread(void *arg) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
