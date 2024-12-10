#include "net/sock/udp.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/af.h"
#include "net/gcoap.h"
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_PORT (20001)
#define SERVER_IP "47.102.103.1"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    int res = sock_udp_create(&sock, &local, NULL, 0);

    if (res < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    char *state_message_work = "work";
    char *state_message_not_work = "not work";

    while (1) {
        if (/* condition to check device state */) {
            sock_udp_send(&sock, state_message_work, strlen(state_message_work), &remote);
        } else {
            sock_udp_send(&sock, state_message_not_work, strlen(state_message_not_work), &remote);
        }
        xtimer_sleep(5);
    }

    return 0;
}