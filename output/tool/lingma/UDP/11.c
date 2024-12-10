#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

// Define the server IP and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

// Define the message to send
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"

// Function to send a message
static void send_message(sock_udp_t *sock, const char *message) {
    // Create a buffer to hold the message
    char buffer[32];
    strncpy(buffer, message, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // Send the message
    ssize_t sent = sock_udp_send(sock, buffer, strlen(buffer), NULL);
    if (sent < 0) {
        printf("Failed to send message: %s\n", strerror(-sent));
    } else {
        printf("Sent message: %s\n", message);
    }
}

// Main function
int main(void) {
    // Initialize the network interface
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    // Convert the server IP address to a binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) < 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Create the UDP socket
    int res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %s\n", strerror(-res));
        return 1;
    }

    // Main loop to send messages every 5 seconds
    while (1) {
        // Determine the message to send
        const char *message = MESSAGE_WORK; // Change this logic as needed

        // Send the message
        send_message(&sock, message);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}