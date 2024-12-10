#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const char *MQTT_BROKER = "47.102.103.1";
const int MQTT_PORT = 1883;
const char *TOPIC = "state";

int main(void) {
    int sockfd;
    struct sockaddr_in broker_addr;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_PORT);
    inet_pton(AF_INET, MQTT_BROKER, &broker_addr.sin_addr);

    // Connect to broker
    if (connect(sockfd, (struct sockaddr*)&broker_addr, sizeof(broker_addr)) < 0) {
        perror("Connection to broker failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // MQTT Connect Packet
    char connect_packet[] = {
        0x10, // Packet type and flags
        0x0C, // Remaining length
        0x00, 0x04, 'M', 'Q', 'T', 'T', // Protocol name
        0x04, // Protocol level
        0x02, // Connect flags
        0x00, 0x3C, // Keep alive
        0x00, 0x00 // Client ID length and data
    };
    send(sockfd, connect_packet, sizeof(connect_packet), 0);

    while (true) {
        // MQTT Publish Packet
        char publish_packet[] = {
            0x30, // Packet type and flags
            0x0D, // Remaining length
            0x00, 0x05, 's', 't', 'a', 't', 'e', // Topic
            0x00, 0x06, 'w', 'o', 'r', 'k', // Payload
            '
'};

        send(sockfd, publish_packet, sizeof(publish_packet), 0);
        xtimer_sleep(5);
    }

    // Disconnect
    char disconnect_packet[] = {0xE0, 0x00};
    send(sockfd, disconnect_packet, sizeof(disconnect_packet), 0);

    close(sockfd);
    return 0;
}