#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

// Define the server IP and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345

// Function to send data to the server
void send_data(const char *data) {
    // Create a UDP endpoint for the server
    sock_udp_ep_t server_ep = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_IP, &server_ep.addr);

    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    // Send data to the server
    ssize_t res = sock_udp_send(&sock, data, strlen(data), &server_ep);
    if (res < 0) {
        printf("Failed to send data: %s\n", strerror(-res));
    } else {
        printf("Sent data: %s\n", data);
    }

    // Close the socket
    sock_udp_close(&sock);
}

// Main function
int main(void) {
    // Initialize the network interface
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(GNRC_NETIF_PID);
    if (!netif) {
        printf("Failed to get network interface\n");
        return 1;
    }

    // Set up a timer to send data every 5 seconds
    while (1) {
        // Check device state (example: always "work")
        const char *device_state = "work";

        // Send the device state to the server
        send_data(device_state);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}