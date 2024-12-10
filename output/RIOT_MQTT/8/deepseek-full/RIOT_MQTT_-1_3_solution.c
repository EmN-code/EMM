
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "paho_mqtt.h"
#include "net/sock.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 5000L

static void network_init(Network *network) {
    network->my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network->my_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &server_addr.sin_addr);

    if (connect(network->my_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to broker");
        exit(EXIT_FAILURE);
    }

    network->mqttread = read;
    network->mqttwrite = write;
    network->disconnect = close;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    network_init(&network);

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != 0) {
        perror("Failed to connect to MQTT broker");
        exit(EXIT_FAILURE);
    }

    message.qos = QOS;
    message.retained = 0;
    message.payload = PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            perror("Failed to publish message");
        }
        sleep(5);
    }

    return 0;
}
