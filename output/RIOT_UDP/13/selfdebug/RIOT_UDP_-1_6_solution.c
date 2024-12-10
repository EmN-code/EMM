#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <arpa/inet.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define SLEEP_INTERVAL (5U * 1000000U)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        printf("Sending message to the server\n");
        sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote);
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}