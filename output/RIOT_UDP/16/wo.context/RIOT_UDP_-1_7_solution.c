#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    const char *server_ip = "47.102.103.1";
    const int server_port = 20001;
    const char *message = "Device state: work";
    const int interval_seconds = 5; // Interval between sends

    int sockfd;
    struct sockaddr_in server_addr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    while (1) {
        // Send message to server
        int n = sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *) &server_addr, sizeof(server_addr));
        if (n < 0) {
            perror("sendto failed");
            return -1;
        }

        printf("Message sent to %s:%d\n", server_ip, server_port);

        // Wait for the specified interval
        sleep(interval_seconds);
    }

    // Close the socket (this code will never actually be reached with the current loop)
    close(sockfd);
    return 0;
}