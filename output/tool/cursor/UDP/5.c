#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

static const char *device_state = "work"; // or "not work"

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET };
    sock_udp_t sock;
    int res;

    // Set the server address and port
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);
    remote.port = SERVER_PORT;

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        res = sock_udp_send(&sock, device_state, strlen(device_state), &remote);
        if (res < 0) {
            puts("Error sending message");
        } else {
            printf("Sent message: \"%s\"\n", device_state);
        }

        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    // Close the socket (unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}