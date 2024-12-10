#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5LU * US_PER_SEC)  // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char *message = "work";
    uint8_t addr[4];

    // Convert string representation of IP address to network format
    if (inet_pton(AF_INET, SERVER_ADDRESS, &addr) != 1) {
        printf("Error: Could not parse IP address\n");
        return 1;
    }
    memcpy(&remote.addr.ipv4, &addr, sizeof(addr));

    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        printf("Error: Could not create UDP sock\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            printf("Error: Could not send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}