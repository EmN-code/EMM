#include <xtimer.h>  // For using delay functions
#include <string.h>   // For using string operations
#include <net/ipv4/addr.h>  // Required for inet_pton to work with RIOT

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

// UDP server details
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Failed to create socket");
        return -1;
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    while (1) {
        // Prepare the device state message
        const char *message = "work";

        // Send the message to the server
        size_t message_len = strlen(message);
        if (sendto(sock, message, message_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send message");
        }

        // Wait for 5 seconds before sending the next message
        xtimer_sleep(5);
    }

    close(sock);
    return 0;
}