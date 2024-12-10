#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/ipv6/addr.h"
#include "arpa/inet.h"
#include "errno.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET6 };
    uint8_t buf[64];

    if (inet_pton(AF_INET6, SERVER_IP, &server.addr.ipv6) != 1) {
        puts("Error: unable to parse IPv6 address");
        return -1;
    }

    server.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &server);
        if (res < 0) {
            printf("Error sending message: %d\n", (int)res);
        }
        else {
            printf("Sent message: %s\n", MESSAGE);
        }
        ztimer_sleep(ZTIMER_USEC, 5 * US_PER_SEC);
    }

    return 0;
}
