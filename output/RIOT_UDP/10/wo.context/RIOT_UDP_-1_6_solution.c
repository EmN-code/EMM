#include <xtimer.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

// Buffer size
#define BUFFER_SIZE 128

// Declare the buffer and UDP endpoint
static char buffer[BUFFER_SIZE];
static sock_udp_ep_t server;

int main(void) {
    printf("Starting UDP client example.\n");

    // initialize the server endpoint struct
    server.family = AF_INET;
    server.port = SERVER_PORT;

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    // Create UDP sock
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return 1;
    }

    // Communication loop
    while (1) {
        // Set the message to be sent
        const char *message = "work";

        // Send the message via UDP every 5 seconds
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);

        // Check if the message was sent correctly
        if (res < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait 5 seconds until next send
        xtimer_sleep(5);
    }

    return 0;
}