#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define TIMER_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

static const char *state_message = "work";

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, state_message, strlen(state_message), &server);
        if (res < 0) {
            printf("Error: unable to send message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}