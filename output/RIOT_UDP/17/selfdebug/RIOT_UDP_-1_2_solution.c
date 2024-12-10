#include <xtimer.h>
#include <string.h>
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr);

    char *state = "work";
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        sock_udp_send(&sock, state, strlen(state), &remote);
        xtimer_sleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}