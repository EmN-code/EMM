#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/af.h"
#include "net/netif.h"
#include "net/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC) // 5 seconds
#define MESSAGE "work or not work"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buf[64];

    // Convert server address to sockaddr structure
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    // Attempt to create a UDP socket until successful
    while (1) {
        if (sock_udp_create(&sock, NULL, NULL, 0) == 0) {
            printf("UDP socket created successfully\n");
            break;
        }
        else {
            printf("Error creating UDP socket, retrying in 1 second...\n");
            xtimer_sleep(1);
        }
    }

    // Main loop to send UDP messages
    while (1) {
        // Send the message
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            printf("Error sending UDP message\n");
        }
        else {
            printf("Message sent: %s\n", MESSAGE);
        }

        // Wait for the defined interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}