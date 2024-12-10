#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define INTERVAL 5

void publish_status(int sockfd, struct sockaddr_in server_addr) {
    const char *work_status = "work or not work";
    int n;

    // Send the work status message
    n = sendto(sockfd, work_status, strlen(work_status), 0, 
                (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("Error sending message");
        return;
    }
    printf("Status published: %s\n", work_status);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        publish_status(sockfd, server_addr);
        sleep(INTERVAL);
    }

    close(sockfd);
    return 0;
}