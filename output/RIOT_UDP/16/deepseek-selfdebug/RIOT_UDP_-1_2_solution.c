
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000U)  // 5 seconds

static const char *device_state = "work";  // Example device state

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Resolve the server IP address
    if (sock_udp_resolve_host(&remote, SERVER_IP) < 0) {
        puts("Error: unable to resolve host");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            puts("Error: unable to send UDP packet");
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL_MS / 1000);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
