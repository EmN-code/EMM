
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(sock_udp_ep_t *remote)
{
    sock_udp_t sock;
    char buf[BUFF_LEN];
    int state = 0; // 0: not work, 1: work

    sock_udp_create(&sock, NULL, remote, 0);

    while (1) {
        if (state == 0) {
            strcpy(buf, "not work");
        } else {
            strcpy(buf, "work");
        }

        printf("Sending: %s\n", buf);
        sock_udp_send(&sock, buf, strlen(buf), NULL);

        state = !state; // Toggle state

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    sock_udp_close(&sock);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    udp_msg_sender(&remote);

    return 0;
}
