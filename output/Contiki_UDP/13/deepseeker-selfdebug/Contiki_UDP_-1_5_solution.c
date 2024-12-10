
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/udp-socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    // Convert IP address string to binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP address");
        return 1;
    }

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send device state to the server
        if (sock_udp_send(&sock, buf, sizeof(buf), NULL) < 0) {
            puts("Error: unable to send UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
