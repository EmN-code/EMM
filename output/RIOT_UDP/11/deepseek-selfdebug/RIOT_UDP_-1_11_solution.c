
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state[] = "work";  // Device state to send

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    uint8_t addr_byte[4];

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &addr_byte) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Set up the remote endpoint
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    memcpy(&remote.addr.ipv4, addr_byte, sizeof(addr_byte));

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent: %s\n", state);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}