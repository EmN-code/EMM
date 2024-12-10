#include <thread.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6 };
    ipv6_addr_t server_addr;

    if (ipv6_addr_from_str(&server_addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }

    remote.port = SERVER_PORT;
    memcpy(&remote.addr.ipv6, &server_addr, sizeof(server_addr));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}