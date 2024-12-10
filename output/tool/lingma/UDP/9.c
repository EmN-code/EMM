#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

// Define the server IP and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345

// Function to send UDP data
static void send_udp_data(sock_udp_t *sock, const char *data) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    // Convert the server IP address to a binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Failed to convert server IP address\n");
        return;
    }

    // Send the data
    ssize_t res = sock_udp_send(sock, data, strlen(data), &remote);
    if (res < 0) {
        printf("Failed to send UDP data: %d\n", (int)res);
    } else {
        printf("Sent %zu bytes to %s:%d\n", res, SERVER_IP, SERVER_PORT);
    }
}

// Function to handle periodic sending
static void periodic_sender(void *arg) {
    sock_udp_t *sock = (sock_udp_t *)arg;
    const char *data = "work";

    while (1) {
        send_udp_data(sock, data);
        xtimer_sleep(5); // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = {
        .family = AF_INET,
        .port = LOCAL_PORT
    };

    // Create the UDP socket
    int res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return 1;
    }

    // Start the periodic sender
    periodic_sender(&sock);

    // Main loop (not needed in this example, but can be used for other tasks)
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}