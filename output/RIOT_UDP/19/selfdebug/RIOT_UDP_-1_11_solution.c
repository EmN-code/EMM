#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define SERVER_PORT (20001)
#define SEND_INTERVAL (5U * US_PER_SEC)

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

int main(void) {
    char* server_ip = "47.102.103.1";
    uint8_t state = 0; // 0: not working, 1: working
    char msg[16];

    // Resolve the server address
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, server_ip) == NULL) {
        printf("Error: unable to parse server IP\n");
        return -1;
    }

    server.netif = SOCK_ADDR_ANY_NETIF;

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        return -1;
    }

    while (1) {
        // Create the message to send
        if (state) {
            strcpy(msg, "working");
            state = 0;
        } else {
            strcpy(msg, "not working");
            state = 1;
        }

        // Send the message
        if (sock_udp_send(&sock, msg, strlen(msg), &server) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent: %s\n", msg);
        }

        // Wait for the next interval
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}