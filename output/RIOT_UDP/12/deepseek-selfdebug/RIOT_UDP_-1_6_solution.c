
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char state[] = "work";  // or "not work" based on device state
    uint8_t addr_bytes[4];

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, addr_bytes) != 1) {
        puts("Error converting IP address");
        return 1;
    }
    memcpy(&remote.addr.ipv4, addr_bytes, 4);

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
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