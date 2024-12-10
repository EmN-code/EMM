#include <string.h>  // Ensure inclusion for string operations.
#include <netinet/in.h>  // Contains network definitions such as AF_INET
#include <stdio.h>  // Standard I/O operations
#include <sys/socket.h> // For socket operations
#include <arpa/inet.h> // For inet_pton

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 1024

void send_device_state(int sock, struct sockaddr_in *server_addr) {
    char *device_state = "work";  // Device state to send
    while (1) {
        sendto(sock, device_state, strlen(device_state), 0, 
               (struct sockaddr *) server_addr, sizeof(*server_addr));
        printf("Sent device state: %s\n", device_state);
        sleep(5);  // Wait for 5 seconds before sending next state
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return -1;
    }

    // Function to send device state every 5 seconds.
    send_device_state(sock, &server_addr);

    return 0;
}