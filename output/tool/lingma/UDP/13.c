#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 0
#define SEND_INTERVAL (5 * 1000000) // 5 seconds in microseconds

// Function to send data to the server
static void send_data(sock_udp_t *sock, const char *data) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT,
    };

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Error: Invalid server IP address\n");
        return;
    }

    // Send the data
    ssize_t res = sock_udp_send(sock, data, strlen(data), &remote);
    if (res < 0) {
        printf("Error: Failed to send data to server\n");
    } else {
        printf("Sent data: %s\n", data);
    }
}

// Main function
int main(void) {
    // Initialize the network interface
    sock_udp_t sock;
    sock_udp_ep_t local = {
        .family = AF_INET,
        .port = LOCAL_PORT,
    };

    // Create a UDP socket
    int res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Error: Failed to create UDP socket\n");
        return 1;
    }

    // Main loop to send data every 5 seconds
    while (1) {
        // Check device state (example: "work" or "not work")
        const char *device_state = "work"; // Change this based on your actual logic

        // Send the device state to the server
        send_data(&sock, device_state);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Close the socket (this will never be reached in the main loop)
    sock_udp_close(&sock);

    return 0;
}