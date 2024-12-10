#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // For inet_pton
#include "net/sock/udp.h"
#include "xtimer.h"

// Define the server IP and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

// Function to send data to the server
static void send_data(const char *state) {
    // Create a UDP endpoint for the server
    sock_udp_ep_t server_ep = {
        .port = SERVER_PORT,
        .family = AF_INET
    };

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4) <= 0) {
        printf("Failed to convert server IP address\n");
        return;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return;
    }

    // Send the data
    ssize_t sent_bytes = sock_udp_send(&sock, state, strlen(state), &server_ep);
    if (sent_bytes < 0) {
        printf("Failed to send data: %d\n", (int)sent_bytes);
    } else {
        printf("Sent %zd bytes to %s:%d\n", sent_bytes, SERVER_IP, SERVER_PORT);
    }

    // Close the socket
    sock_udp_close(&sock);
}

// Main function
int main(void) {
    // Initialize the network
    if (gnrc_netif_init() < 0) {
        printf("Failed to initialize network interface\n");
        return -1;
    }

    // Main loop to send data every 5 seconds
    while (1) {
        // Simulate device state (work or not work)
        const char *state = "work"; // Change this to "not work" as needed

        // Send the device state to the server
        send_data(state);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}