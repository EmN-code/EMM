#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"

static void udp_client_send(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    ssize_t sent_bytes;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    // Configure the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Use inet_pton to convert the IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return;
    }

    // Send the message to the server
    sent_bytes = sendto(sock, MESSAGE, strlen(MESSAGE), 0,
                        (const struct sockaddr *)&server_addr, sizeof(server_addr));
                        
    if (sent_bytes < 0) {
        perror("sendto");
    } else {
        printf("Sent %zd bytes to %s:%d\n", sent_bytes, SERVER_IP, SERVER_PORT);
    }

    // Close the socket
    close(sock);
}

int main(void) {
    while (1) {
        udp_client_send();
        usleep(5000000); // Sleep for 5 seconds
    }
    return 0;
}