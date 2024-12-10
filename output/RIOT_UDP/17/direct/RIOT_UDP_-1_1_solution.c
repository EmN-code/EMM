#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5 // Interval in seconds

int main() {
    struct sockaddr_in server_addr;
    int sockfd;
    char *device_state = "work"; // The state can be switched to "not work" as needed
    
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        return 1;
    }

    // Setup server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    while (1) {
        // Send device status to the server
        int n = sendto(sockfd, device_state, strlen(device_state), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if (n < 0) {
            perror("Failed to send message");
        }
        else {
            printf("Device state '%s' sent to %s:%d\n", device_state, SERVER_IP, SERVER_PORT);
        }

        // Wait for 5 seconds
        sleep(INTERVAL);
    }

    // Close socket
    close(sockfd);
    return 0;
}