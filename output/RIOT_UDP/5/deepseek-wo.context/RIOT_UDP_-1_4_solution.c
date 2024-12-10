
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buf[64];
    int res;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        strcpy(buf, DEVICE_STATE);
        res = sock_udp_send(&sock, buf, strlen(buf), &remote);
        if (res < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        xtimer_sleep(INTERVAL_SEC);
    }

    sock_udp_close(&sock);
    return 0;
}
