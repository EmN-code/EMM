
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define KEEP_ALIVE 60

typedef struct {
    int (*mqttread)(void*, unsigned char*, int, int);
    int (*mqttwrite)(void*, unsigned char*, int, int);
    int socket;
} Network;

int mqttread(void *n, unsigned char *buf, int len, int timeout) {
    Network *network = (Network *)n;
    return recv(network->socket, buf, len, 0);
}

int mqttwrite(void *n, unsigned char *buf, int len, int timeout) {
    Network *network = (Network *)n;
    return send(network->socket, buf, len, 0);
}

void NetworkInit(Network *n) {
    n->mqttread = mqttread;
    n->mqttwrite = mqttwrite;
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void NetworkConnect(Network *n, const char *address, int port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    if (connect(n->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

void NetworkDisconnect(Network *n) {
    close(n->socket);
}

int main() {
    Network network;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClient_create(&client, &network, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = KEEP_ALIVE;
    conn_opts.cleansession = 1;

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        exit(EXIT_FAILURE);
    }

    pubmsg.payload = (void *)PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    while (1) {
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Message '%s' with token value %d delivered\n", PAYLOAD, token);
        sleep(5);
    }

    NetworkDisconnect(&network);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
