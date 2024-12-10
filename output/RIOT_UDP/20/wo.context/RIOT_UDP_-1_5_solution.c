#include "ztimer.h"
#include "sock/udp.h"
#include "xtimer.h"
#include <stdio.h>
#include <stdlib.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * ZTIMER_MSEC_PER_SEC)

static char message[] = "Device state: work";

int main(void) {
    /* Initialize the UDP endpoint */
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT
    };

    /* Convert the server IP address string into a usable format */
    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6) != 1) {
        printf("Error: Unable to parse the server IP address\n");
        return 1;
    }

    /* Open a UDP connection */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to open UDP socket\n");
        return 1;
    }

    /* Periodic message sending loop */
    while (1) {
        /* Send the message to the server */
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            printf("Error: Unable to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        /* Wait for the specified interval */
        ztimer_sleep(ZTIMER_MSEC, MESSAGE_INTERVAL);
    }

    return 0;
}