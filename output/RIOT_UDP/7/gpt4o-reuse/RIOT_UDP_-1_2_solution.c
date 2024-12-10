
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    while (1) {
        char buf[BUFF_LEN];
        strcpy(buf, "work"); // or "not work" based on device state
        printf("Sending: %s\n", buf);
        ssize_t res = sock_udp_send(sock, buf, strlen(buf), remote);
        if (res < 0) {
            printf("Failed to send UDP message\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    local.port = 0; // Let the system assign a local port
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    if (sock_udp_str2ep(&remote, SERVER_IP) < 0) {
        printf("Error parsing server address\n");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    udp_msg_sender(&sock, &remote);

    sock_udp_close(&sock);
    return 0;
}
