#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define BUFFER_SIZE 64
#define SLEEP_INTERVAL 5000000  /* in microseconds, 5000000us = 5s */

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buffer[BUFFER_SIZE];
    int sock_result;

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting IP address.");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket.");
        return 1;
    }

    while(1) {
        // Prepare the message to be sent
        strncpy(buffer, MESSAGE, BUFFER_SIZE);

        // Send the message
        sock_result = sock_udp_send(&sock, buffer, strlen(MESSAGE), &remote);
        if (sock_result < 0) {
            printf("Error sending UDP message: %d\n", sock_result);
        } else {
            printf("Sent message: %s\n", MESSAGE);
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);
    return 0;
}