#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define TIMER_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    uint8_t server_addr[16];
    if (inet_pton(AF_INET, SERVER_IP, &server_addr) != 1) {
        puts("Error: unable to parse server IP");
        return 1;
    }
    memcpy(server.addr.ipv6, server_addr, 16);

    sock_udp_t sock;
    uint8_t buf[128];
    sprintf((char *)buf, "device state: work");
    size_t len = strlen((char *)buf);

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, buf, len, &server) < 0) {
            puts("Error: unable to send data");
            return 1;
        }
        xtimer_sleep(TIMER_INTERVAL);
        sprintf((char *)buf, "device state: not work");
    }

    return 0;
}