#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 1024

int main() {
    // Create a UDP socket
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;

    // Filling server information
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Sending device state every 5 seconds
    while (1) {
        const char *device_state_msg = "work"; // Or "not work" depending on the state

        int n = sendto(sockfd, (const char *)device_state_msg, strlen(device_state_msg),
                       MSG_CONFIRM, (const struct sockaddr *)&servaddr,
                       sizeof(servaddr));

        if (n < 0) {
            perror("Failed to send message");
        } else {
            printf("Device state '%s' sent to %s:%d\n", device_state_msg, SERVER_ADDR, SERVER_PORT);
        }

        // Wait for 5 seconds
        sleep(5);
    }

    // Close the socket
    close(sockfd);
    return 0;
}