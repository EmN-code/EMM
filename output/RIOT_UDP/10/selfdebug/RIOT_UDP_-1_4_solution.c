#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[20];
    int interval = 5; // Send every 5 seconds

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    
    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(20001);
    if(inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        return -1;
    }

    while (1) {
        snprintf(message, sizeof(message), "Device is working");
        
        // Send UDP message
        sendto(sockfd, (const char *)message, strlen(message), 
            MSG_CONFIRM, (const struct sockaddr *) &server_addr, 
                sizeof(server_addr));
        printf("Message sent.\n");

        sleep(interval);
    }

    close(sockfd);
    return 0;
}