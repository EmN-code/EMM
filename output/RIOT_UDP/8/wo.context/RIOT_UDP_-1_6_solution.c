#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define SERVER_ADDR    "47.102.103.1"
#define SERVER_PORT    20001
#define SEND_INTERVAL  (5U * US_PER_SEC)

enum DeviceState {
    WORK = 0,
    NOT_WORK
};

int main(void) {
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    enum DeviceState state = WORK;
    while (1) {
        const char *msg = (state == WORK) ? "Device is working" : "Device is not working";
        sock_udp_send(&sock, msg, strlen(msg), &remote);
        state = (state == WORK) ? NOT_WORK : WORK;
        xtimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}