#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "kernel.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

static void udp_send(sock_udp_ep_t *server, const char *state) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    if (sock_udp_send(&sock, state, strlen(state), server) < 0) {
        printf("Error sending UDP packet\n");
    } else {
        printf("Sent UDP packet: %s\n", state);
    }

    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        printf("Error parsing server address\n");
        return 1;
    }

    char *state_work = "work";
    char *state_not_work = "not work";
    bool is_working = true;

    while (1) {
        if (is_working) {
            udp_send(&server, state_work);
        } else {
            udp_send(&server, state_not_work);
        }

        is_working = !is_working;
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}