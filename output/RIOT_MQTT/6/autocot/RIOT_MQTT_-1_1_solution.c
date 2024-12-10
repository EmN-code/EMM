#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL 5
#define MQTT_BUFFER_SIZE 1024

void mqtt_publish(int sock, const char *topic, const char *message) {
    char buffer[MQTT_BUFFER_SIZE];
    memset(buffer, 0, MQTT_BUFFER_SIZE);
    snprintf(buffer, MQTT_BUFFER_SIZE, "PUBLISH %s %s", topic, message); // Simplified publish format
    send(sock, buffer, strlen(buffer), 0);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_SERVER_PORT);

    if (inet_pton(AF_INET, MQTT_SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        return EXIT_FAILURE;
    }

    printf("Connected to MQTT Broker\n");

    while (1) {
        mqtt_publish(sock, "device/state", "work");
        sleep(PUBLISH_INTERVAL);
        mqtt_publish(sock, "device/state", "not work");
        sleep(PUBLISH_INTERVAL);
    }

    close(sock);
    return EXIT_SUCCESS;
}