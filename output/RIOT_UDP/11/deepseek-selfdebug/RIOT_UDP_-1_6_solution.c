
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    char buf[] = DEVICE_STATE;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            puts("Error: Failed to send UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
