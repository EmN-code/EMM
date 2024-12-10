
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // Change this to "not work" if needed
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

int main(void)
{
    // Initialize the server endpoint with the given IP address
    ipv4_addr_t server_addr;
    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return 1;
    }
    server.addr.ipv4 = server_addr;

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    char buf[64];
    while (1) {
        // Prepare the message to send
        snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sock_udp_send(&sock, buf, strlen(buf), &server) < 0) {
            puts("Error: failed to send UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
