
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
    struct sockaddr_in broker;
} MQTTClient;

void init_mqtt_client(MQTTClient *client) {
    client->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sock < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&client->broker, 0, sizeof(client->broker));
    client->broker.sin_family = AF_INET;
    client->broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &client->broker.sin_addr);
}

void connect_to_broker(MQTTClient *client) {
    if (connect(client->sock, (struct sockaddr *)&client->broker, sizeof(client->broker)) < 0) {
        perror("Failed to connect to broker");
        exit(EXIT_FAILURE);
    }
}

void publish_message(MQTTClient *client, const char *topic, const char *payload, int qos) {
    char message[100];
    snprintf(message, sizeof(message), "PUBLISH %s %s %d", topic, payload, qos);
    send(client->sock, message, strlen(message), 0);
}

void disconnect_from_broker(MQTTClient *client) {
    close(client->sock);
}

int main() {
    MQTTClient client;
    init_mqtt_client(&client);
    connect_to_broker(&client);

    while (true) {
        publish_message(&client, TOPIC, PAYLOAD, QOS);
        sleep(5);
    }

    disconnect_from_broker(&client);
    return 0;
}
