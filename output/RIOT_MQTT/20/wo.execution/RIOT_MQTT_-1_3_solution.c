#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "paho_mqtt.h"

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    unsigned int addr_len = sizeof(server_addr);
    char message[256];
    int result;

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    // Define the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_SERVER_PORT);
    inet_pton(AF_INET, MQTT_SERVER_IP, &server_addr.sin_addr);

    // Connect to the server
    result = connect(sock, (struct sockaddr *)&server_addr, addr_len);
    if (result < 0) {
        perror("connect");
        close(sock);
        return -1;
    }

    // Main loop to publish the message
    while (1) {
        snprintf(message, sizeof(message), "work or not work");

        // Send message to the server
        result = send(sock, message, strlen(message), 0);
        if (result < 0) {
            perror("send");
            close(sock);
            return -1;
        }

        // Wait for the specified interval
        sleep(MQTT_PUBLISH_INTERVAL);
    }

    // Close the socket
    close(sock);
    return 0;
}