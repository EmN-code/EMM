
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false

typedef struct {
    int socket;
    struct sockaddr_in broker_addr;
} Network;

void NetworkInit(Network *n) {
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&n->broker_addr, 0, sizeof(n->broker_addr));
    n->broker_addr.sin_family = AF_INET;
    n->broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &n->broker_addr.sin_addr);
}

void MQTTClientInit(Network *network) {
    if (connect(network->socket, (struct sockaddr *)&network->broker_addr, sizeof(network->broker_addr)) < 0) {
        perror("Failed to connect to MQTT broker");
        exit(EXIT_FAILURE);
    }
}

void MQTTPublish(Network *network, const char *topic, const char *payload) {
    uint8_t packet[100];
    int packet_len = 0;

    // Fixed header
    packet[packet_len++] = 0x30; // MQTT PUBLISH packet
    packet[packet_len++] = strlen(topic) + strlen(payload) + 2; // Remaining length

    // Variable header
    packet[packet_len++] = (uint8_t)(strlen(topic) >> 8); // Topic length MSB
    packet[packet_len++] = (uint8_t)(strlen(topic) & 0xFF); // Topic length LSB
    memcpy(&packet[packet_len], topic, strlen(topic)); // Topic
    packet_len += strlen(topic);

    // Payload
    memcpy(&packet[packet_len], payload, strlen(payload)); // Payload
    packet_len += strlen(payload);

    send(network->socket, packet, packet_len, 0);
}

int main(void) {
    Network network;
    NetworkInit(&network);
    MQTTClientInit(&network);

    while (1) {
        MQTTPublish(&network, MQTT_TOPIC, MQTT_PAYLOAD);
        sleep(5);
    }

    close(network.socket);
    return 0;
}
