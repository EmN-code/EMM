#include "net/sock/udp.h"
#include "xtimer.h"
#include "string.h"

#define SERVER_ADDR   "47.102.103.1"
#define SERVER_PORT   20001
#define INTERVAL      5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    char msg[] = "work";
    while (1) {
        sock_udp_send(&sock, msg, strlen(msg), &remote);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}