#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/sock_udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_PERIOD 5

int main(void)
{
    sock_udp_ep_t server = {.family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT};
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    char *msg = "Device state: work";
    while (1) {
        if (sock_udp_send(&sock, msg, strlen(msg), &server) < 0) {
            printf("Error: unable to send message\n");
        }
        xtimer_sleep(MESSAGE_PERIOD);
    }

    return 0;
}