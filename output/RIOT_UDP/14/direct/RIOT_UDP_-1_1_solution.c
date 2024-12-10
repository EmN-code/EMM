#include <x/IOTA/xIota>  // Include the necessary RIOT libraries
#include <sys/socket.h>       // For socket communication
#include <netinet/in.h>      // For sockaddr_in and basic networking 
#include <stdio.h>           // For printf and other I/O operations
#include <string.h>          // For string operations
#include <unistd.h>          // For close() function

#define SERVER_IP "47.102.103.1"  // IP address of the server
#define SERVER_PORT 20001          // Port number of the server
#define BUFFER_SIZE 1024          // Buffer size for sending messages

void delay(unsigned int secs) {
    // Simple sleep function
    unsigned int retTime = time(0) + secs;   
    while (time(0) < retTime);
}

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    const char *device_state = "Device state: working";
    
    // Creating a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    // Defining server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Sending device state every 5 seconds
    while (1) {
        printf("Sending device state to server...\n");
        int n = sendto(sockfd, device_state, strlen(device_state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (n < 0) {
            printf("Error sending data\n");
        } else {
            printf("Data sent successfully\n");
        }

        // Wait for 5 seconds
        delay(5);
    }

    // Close the socket
    close(sockfd);
    return 0;
}