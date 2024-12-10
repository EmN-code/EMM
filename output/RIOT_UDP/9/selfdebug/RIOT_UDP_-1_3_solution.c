#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/udp.h"
#include "net/af.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5

int main(void) {
    printf("Starting UDP client example...\n");

    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

    char message[] = "work";
    ipv4_addr_t server_addr;
    int result;

    // Convert the IP address from string to the byte representation
    result = inet_pton(AF_INET, SERVER_IP, &server_addr);
    if (result != 1) {
        printf("Error occurred in inet_pton, result: %d\n", result);
        return -1;
    }
    server.addr.ipv4 = server_addr;

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    while (1) {
        // Send message to the server
        if (sock_udp_send(&sock, message, sizeof(message), &server) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        // Wait before sending the next message
        sleep(SEND_INTERVAL);
    }

    // This line will actually never be reached
    sock_udp_close(&sock);
    return 0;
}