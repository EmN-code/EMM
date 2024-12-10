#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static sock_udp_ep_t server;

int main(void)
{
    sock_udp_t sock;
    char data[] = "work";
    size_t data_len = sizeof(data);

    // Initialize server endpoint
    server.family = AF_INET6;  // Set IPv6 to avoid IPV4
    ipv6_addr_from_str((ipv6_addr_t *)server.addr.ipv6, SERVER_ADDR);
    server.port = SERVER_PORT;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send data to server
        if (sock_udp_send(&sock, data, data_len, &server) < 0) {
            puts("Failed to send data");
        } else {
            puts("Data sent successfully");
        }

        // Wait for the defined interval
        xtimer_sleep(INTERVAL / US_PER_SEC);  // Convert to seconds
    }

    return 0;
}