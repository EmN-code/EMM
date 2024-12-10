
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void send_coap_request(const char *ip, uint16_t port, const char *path, const char *message) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int len;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Prepare the CoAP request
    len = snprintf(buffer, sizeof(buffer), "POST %s HTTP/1.1\r\nHost: %s\r\nContent-Length: %zu\r\n\r\n%s",
                   path, ip, strlen(message), message);

    // Send the CoAP request
    if (sendto(sockfd, buffer, len, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Close the socket
    close(sockfd);
}

int main() {
    send_coap_request(SERVER_IP, SERVER_PORT, URI_PATH, MESSAGE);
    return 0;
}
