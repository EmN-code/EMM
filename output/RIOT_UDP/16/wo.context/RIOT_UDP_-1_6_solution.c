#include <xtimer.h>  // Include the necessary RIOT library for timing functions
#include <stdio.h>    // Standard I/O library for printing messages
#include <string.h>   // String operations library
#include <net/sock/udp.h>  // UDP socket library
#include <net/ipv4/addr.h>  // IPV4 address manipulation library

#define SERVER_PORT 20001
#define SERVER_ADDRESS "47.102.103.1"
#define PERIOD 5

// Main function
int main(void) {
    // Print an informative message
    printf("Starting UDP client on RIOT OS for ESP32\n");

    // Define remote server endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert input IP address\n");
        return -1;
    }

    // Initialize the UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Could not create UDP socket\n");
        return -1;
    }

    // Device state message
    const char *message = "Device is working";

    // Main loop for sending UDP messages
    while (1) {
        // Send the state message to the UDP server
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            printf("Error: Could not send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for the specified period (5 seconds)
        xtimer_sleep(PERIOD);
    }

    // Cleanup (although we never reach here)
    sock_udp_close(&sock);

    return 0;
}