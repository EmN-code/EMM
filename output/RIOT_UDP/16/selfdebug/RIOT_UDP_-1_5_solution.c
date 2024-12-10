#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buffer[64];
    size_t buffer_len;
    int res;

    // Convert IP address from string to binary form
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create the socket");
        return 1;
    }

    // Device state to send
    const char *state = "work";

    while (1) {
        // Prepare the message
        memset(buffer, 0, sizeof(buffer));
        buffer_len = snprintf((char *)buffer, sizeof(buffer), "Device state: %s", state);
        
        // Send the message
        res = sock_udp_send(&sock, buffer, buffer_len, &remote);
        if (res < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}