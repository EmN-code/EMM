#include <xtimer.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/af.h>
#include "msg.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    20001
#define MESSAGE        "work"
#define INTERVAL       (5U * US_PER_SEC)

static msg_t msg_queue[8];
static msg_t msg;

int main(void) {
    msg_init_queue(msg_queue, 8);

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t state = 1;

    int res = inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv4);
    if (res <= 0) {
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        return -1;
    }

    while (1) {
        const char *msg_to_send = state ? MESSAGE : "not work";
        sock_udp_send(&sock, msg_to_send, strlen(msg_to_send), &remote);

        state = !state;  // Toggle state for the next send
        xtimer_sleep(INTERVAL);
    }

    return 0;
}