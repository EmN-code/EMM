
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL 5  // in seconds

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char state[10];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Main loop to send device state every 5 seconds
    while (true) {
        // Simulate device state
        if (rand() % 2 == 0) {
            strcpy(state, DEVICE_STATE_WORK);
        } else {
            strcpy(state, DEVICE_STATE_NOT_WORK);
        }

        // Send the state to the server
        if (sendto(sockfd, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
        } else {
            printf("Sent: %s\n", state);
        }

        // Wait for 5 seconds before sending the next state
        sleep(SEND_INTERVAL);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
