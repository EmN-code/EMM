#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000 // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_device_state(const char *state) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "State: %s", state);

    // Send the data
    ssize_t res = sock_udp_send(&sock, buffer, strlen(buffer), &remote);
    if (res < 0) {
        printf("Failed to send data: %d\n", (int)res);
    } else {
        printf("Sent data: %s\n", buffer);
    }
}

void periodic_sender(void *arg) {
    while (1) {
        // Check device state (for demonstration, we'll assume it's always working)
        const char *state = "work";

        // Send the device state
        send_device_state(state);

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / 1000000);
    }
}

int main(void) {
    // Initialize the network interface
    if (gnrc_netif_get_by_pid(IPV4_PID) == NULL) {
        printf("No IPv4 network interface found\n");
        return 1;
    }

    // Create a UDP socket
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return 1;
    }

    // Set the remote endpoint
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Start the periodic sender
    thread_create(periodic_sender, NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, "periodic_sender");

    // Main loop (optional, can be removed if not needed)
    while (1) {
        xtimer_sleep(1000000); // Sleep for 1 second
    }

    return 0;
}