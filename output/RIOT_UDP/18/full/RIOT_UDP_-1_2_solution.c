#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 64

int main(void) {
    struct sockaddr_in server_addr;
    int sockfd;
    char buffer[BUF_SIZE];
    int ret;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // Attempt to connect to the UDP server in a loop
    while (1) {
        // Prepare the message
        const char *message = "work";
        printf("Sending message: %s\n", message);

        ret = sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            perror("sendto failed");
        } else {
            printf("Message sent successfully\n");
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    // Close the socket
    close(sockfd);
    return 0;
}