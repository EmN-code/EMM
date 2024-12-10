#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "net/ipv4/addr.h"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000  // 5 seconds in microseconds

char device_state[] = "work";  // Initial state
static void init_udp_socket(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_UDP_EP_ANY;
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    // Parse the server IP address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) != 0) {
        printf("Failed to parse server IP address\n");
        return;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return;
    }

    // Store the socket for later use
    static sock_udp_t *udp_socket = &sock;
}
static void send_device_state(sock_udp_t *sock) {
    char message[10];
    snprintf(message, sizeof(message), "%s", device_state);

    // Send the message
    ssize_t sent = sock_udp_send(sock, message, strlen(message), NULL);
    if (sent < 0) {
        printf("Failed to send message: %d\n", (int)sent);
    } else {
        printf("Sent message: %s\n", message);
    }
}
int main(void) {
    // Initialize the UDP socket
    init_udp_socket();

    // Main loop
    while (1) {
        // Send the device state
        send_device_state(udp_socket);

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / 1000000);
    }

    return 0;
}