// Import necessary libraries for UDP and timing functionalities
#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/udp.h"
#include "net/netif.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static void send_udp(const char *message) {
    ipv6_addr_t remote_addr;
    char addr_str[] = SERVER_IP;
    uint16_t port = SERVER_PORT;
    int res = ipv6_addr_from_str(&remote_addr, addr_str);
    if (res == NULL) {
        printf("Error in address conversion\n");
        return;
    }

    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = port
    };
    memcpy(&remote.addr.ipv6, &remote_addr, sizeof(remote_addr));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
        printf("Error sending message\n");
    } else {
        printf("Message sent: %s\n", message);
    }

    sock_udp_close(&sock);
}

int main(void) {
    // Initial arbitrary state, could be based on actual device check
    const char *state = "work";

    // Infinite loop sending the state every 5 seconds
    while (1) {
        send_udp(state);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}