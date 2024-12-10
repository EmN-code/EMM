
#include <stdio.h>
#include <xtimer.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000000

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    // Convert IP address string to binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, UDP_SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP address");
        return 1;
    }

    // Open a UDP socket
    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, buf, sizeof(buf), &server) < 0) {
            puts("Error: unable to send UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
