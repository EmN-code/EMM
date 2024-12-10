
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 20

typedef struct {
    int sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} Network;

typedef struct {
    Network network;
    unsigned int command_timeout_ms;
    unsigned char* sendbuf;
    size_t sendbuf_size;
    unsigned char* readbuf;
    size_t readbuf_size;
} MQTTClient;

int mqttread(Network* n, unsigned char* buffer, int len, int timeout) {
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    setsockopt(n->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
    return recv(n->sock, buffer, len, 0);
}

int mqttwrite(Network* n, unsigned char* buffer, int len, int timeout) {
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    setsockopt(n->sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));
    return send(n->sock, buffer, len, 0);
}

void MQTTClientInit(MQTTClient* client, Network* network, unsigned int command_timeout_ms,
                    unsigned char* sendbuf, size_t sendbuf_size, unsigned char* readbuf, size_t readbuf_size) {
    client->network = *network;
    client->command_timeout_ms = command_timeout_ms;
    client->sendbuf = sendbuf;
    client->sendbuf_size = sendbuf_size;
    client->readbuf = readbuf;
    client->readbuf_size = readbuf_size;
}

int MQTTConnect(MQTTClient* client) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &serv_addr.sin_addr);

    client->network.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client->network.sock < 0) {
        perror("socket");
        return -1;
    }

    if (connect(client->network.sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return -1;
    }

    return 0;
}

int MQTTPublish(MQTTClient* client, const char* topic, const char* payload) {
    char message[100];
    snprintf(message, sizeof(message), "PUBLISH %s %s", topic, payload);
    return send(client->network.sock, message, strlen(message), 0);
}

int main() {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100];
    unsigned char readbuf[100];

    network.mqttread = mqttread;
    network.mqttwrite = mqttwrite;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (MQTTConnect(&client) < 0) {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        if (MQTTPublish(&client, TOPIC, PAYLOAD) < 0) {
            fprintf(stderr, "Failed to publish message\n");
        }
        sleep(5);
    }

    close(client.network.sock);
    return 0;
}
