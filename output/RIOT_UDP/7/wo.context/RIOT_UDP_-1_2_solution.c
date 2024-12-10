#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5U  // interval in seconds

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    uint8_t state_not_work[11] = "not work";
    uint8_t state_work[5] = "work";

    // Resolve remote address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: Unable to parse remote address");
        return 1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Change the state message between 'work' and 'not work'
        uint8_t *state = (xtimer_now_usec() / (MESSAGE_INTERVAL * US_PER_SEC) % 2) == 0 ? state_not_work : state_work;
        
        ssize_t res = sock_udp_send(&sock, state, strlen((char *)state), &remote);
        if (res < 0) {
            puts("Error: unable to send message");
        } else {
            printf("Message sent: %s\n", state);
        }

        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}