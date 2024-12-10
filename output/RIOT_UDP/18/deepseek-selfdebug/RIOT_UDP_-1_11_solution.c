
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

int main(void)
{
    sock_udp_t sock;
    char buf[] = "work";  // Example device state
    uint8_t addr_bytes[sizeof(struct in_addr)];

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, addr_bytes) <= 0) {
        puts("Error: Invalid IP address");
        return 1;
    }

    // Set the server address
    memcpy(&server.addr.ipv4, addr_bytes, sizeof(server.addr.ipv4));

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, buf, strlen(buf), &server) < 0) {
            puts("Error: Failed to send UDP packet");
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
