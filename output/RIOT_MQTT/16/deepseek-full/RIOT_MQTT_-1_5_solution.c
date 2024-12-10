
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEP_ALIVE 20
#define MQTT_QOS 1
#define MQTT_RETAIN 0

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

    memset(&n->server_addr, 0, sizeof(n->server_addr));
    n->server_addr.sin_family = AF_INET;
    n->server_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &n->server_addr.sin_addr);
}

int NetworkConnect(Network *n) {
    if (connect(n->socket, (struct sockaddr *)&n->server_addr, sizeof(n->server_addr)) < 0) {
        perror("Failed to connect to MQTT broker");
        return -1;
    }
    return 0;
}

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen((char *)message.payload);

    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    if (NetworkConnect(&network) < 0) {
        return EXIT_FAILURE;
    }

    unsigned char sendbuf[256], readbuf[256];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEP_ALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        return EXIT_FAILURE;
    }

    while (1) {
        publish_state(&client);
        sleep(5);
    }

    return EXIT_SUCCESS;
}
