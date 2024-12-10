#include <xtimer.h>  // For timing operations
#include <stdio.h>    // For printf and standard input/output operations
#include <net/af.h>   // For address family definitions
#include <net/sock/udp.h>  // For UDP socket operations

#define UDP_SERVER_ADDRESS      "47.102.103.1"
#define UDP_SERVER_PORT         20001
#define SEND_INTERVAL           (5U * US_PER_SEC)  // 5 seconds in microseconds

static const char *device_state = "work";  // Define device state

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };  // Initialize remote object
    remote.port = UDP_SERVER_PORT;  // Set the UDP server port number
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER_ADDRESS);  // Convert IP address to binary format

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        // Send device state to server each interval
        if (sock_udp_send(&sock, device_state, sizeof(device_state), &remote) < 0) {
            puts("Error: Unable to send message\n");
        } else {
            printf("Sent: %s\n", device_state);
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    // Clean up the socket
    sock_udp_close(&sock);
    return 0;
}