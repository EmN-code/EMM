
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/inet_pton.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char buf[64];

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    remote.port = SERVER_PORT;

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send "work or not work" message
        strcpy(buf, "work or not work");
        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
