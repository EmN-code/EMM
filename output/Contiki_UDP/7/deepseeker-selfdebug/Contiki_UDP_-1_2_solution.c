
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    // Initialize the remote IP address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, UDP_SERVER_ADDR) == NULL) {
        puts("Error: invalid server address");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the UDP server
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
