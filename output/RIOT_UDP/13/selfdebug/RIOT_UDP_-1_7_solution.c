#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include "xtimer.h"

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     20001
#define MESSAGE         "Device state: work"
#define MESSAGE_INTERVAL   (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    uint8_t addr_family = AF_INET;
    int res = inet_pton(addr_family, SERVER_ADDR, &remote.addr.ipv4);

    if (res != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            printf("Failed to send UDP message\n");
        } else {
            printf("Message sent: %s\n", MESSAGE);
        }

        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    // Normally we would close the socket here, but since this is an infinite loop 
    // for the demonstration, it’s left out intentionally.
}
