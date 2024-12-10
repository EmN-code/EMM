#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 8888
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds
#define STATE_MSG "work or not work"

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = LOCAL_PORT;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP sock");
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, STATE_MSG, sizeof(STATE_MSG), &remote);
        if (res < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent");
        }

        xtimer_sleep(INTERVAL);
    }

    // Normally we'd clean up, but loop is infinite
    return 0;
}