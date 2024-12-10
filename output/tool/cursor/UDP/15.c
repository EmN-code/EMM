#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET };
    sock_udp_t sock;
    uint8_t buf[64];
    ssize_t res;

    // Set the server address and port
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP sock");
        return 1;
    }

    while (1) {
        // Prepare the message
        const char *message = "work"; // or "not work"
        size_t msg_len = strlen(message);

        // Send the message to the server
        if (sock_udp_send(&sock, message, msg_len, &remote) < 0) {
            puts("Error sending message");
        } else {
            printf("Sent message: \"%s\"\n", message);
        }

        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    // Close the socket (unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}s