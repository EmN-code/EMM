// This program connects to a UDP server at 47.102.103.1:20001 and sends
// the device state ("work" or "not work") every five seconds.

#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

int main(void) {
    printf("UDP client example for ESP32 on RIOT\n");

    // Define the UDP server endpoint
    sock_udp_ep_t server = { .family = AF_INET, .port = 20001 };
    inet_pton(AF_INET, "47.102.103.1", &server.addr.ipv4);

    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Define device state messages
    const char *message_working = "work";
    const char *message_not_working = "not work";
    const char *message = message_working; // Set default message

    // Infinite loop to send UDP messages
    while (1) {
        ssize_t bytes_sent = sock_udp_send(&sock, message, strlen(message), &server);
        if (bytes_sent < 0) {
            printf("Error sending packet: %ld\n", bytes_sent);
        } else {
            printf("Sent message: %s\n", message);
        }
        // Wait for 5 seconds
        usleep(5000000);

        // Toggle message just to show some change in message
        message = (message == message_working) ? message_not_working : message_working;
    }

    // Close socket (though we will never reach here)
    sock_udp_close(&sock);
    return 0;
}