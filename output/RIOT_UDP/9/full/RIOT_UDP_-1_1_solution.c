#include <xtimer.h>  // Timer functionalities
#include <stdio.h>
#include <sys/socket.h>  // Socket APIs
#include <net/af.h>  // Address family definitions
#include <net/ipv6/addr.h>  // IPv6 support
#include <net/sock/udp.h>  // UDP support
#include <net/if.h>  // Network interfaces
#include <string.h>  // String utilities

#define UDP_SERVER_ADDR "47.102.103.1"  // Server address
#define UDP_SERVER_PORT 20001  // Server port
#define INTERVAL 5  // Send interval in seconds

int main(void) {
    // Create a socket
    sock_udp_ep_t remote;
    sock_udp_t socket;
    char message[] = "work";  // Message to send

    // Set up remote address
    memset(&remote, 0, sizeof(remote));
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER_ADDR);
    remote.family = AF_INET6;  // Use IPv6
    remote.port = UDP_SERVER_PORT;

    // Initialize UDP socket
    if (sock_udp_create(&socket, NULL, NULL, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    while (1) {
        // Try sending the message repeatedly until successful
        while (sock_udp_send(&socket, message, strlen(message), &remote) < 0) {
            puts("Failed to send message, retrying...");
            xtimer_sleep(1);  // Wait for a second before retrying
        }
        puts("Message sent successfully");

        // Wait for the specified interval
        xtimer_sleep(INTERVAL);
    }

    // Close the socket
    sock_udp_close(&socket);

    return 0;
}