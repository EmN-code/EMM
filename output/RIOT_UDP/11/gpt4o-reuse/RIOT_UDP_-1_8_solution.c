
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512
#define DEVICE_STATE "work"  // or "not work"

void udp_msg_sender(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    char buf[BUFF_LEN];
    while (1) {
        memset(buf, 0, BUFF_LEN);
        sprintf(buf, "Device state: %s", DEVICE_STATE);
        printf("Sending: %s\n", buf);

        ssize_t res = sock_udp_send(sock, buf, strlen(buf), remote);
        if (res < 0) {
            printf("Failed to send UDP message\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
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
        printf("Failed to create UDP socket\n");
        return 1;
    }

    udp_msg_sender(&sock, &remote);

    sock_udp_close(&sock);
    return 0;
}
