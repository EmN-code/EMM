
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512
#define DELAY_SECONDS 5

void udp_msg_sender(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    char buf[BUFF_LEN];
    while (1)
    {
        strcpy(buf, "work"); // Example device state
        printf("Sending: %s\n", buf);
        sock_udp_send(sock, buf, strlen(buf), remote);
        xtimer_sleep(DELAY_SECONDS);
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    udp_msg_sender(&sock, &remote);

    sock_udp_close(&sock);
    return 0;
}
