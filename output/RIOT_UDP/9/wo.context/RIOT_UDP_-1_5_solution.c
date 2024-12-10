#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &server.addr.ipv6);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Could not create UDP socket\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &server) < 0) {
            printf("Error: Could not send message\n");
        } else {
            printf("Message sent\n");
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}