#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    int rc;
    char message[] = "Device state: working";

    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address.");
        return 1;
    }

    while (1) {
        rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc < 0) {
            puts("Error: unable to create UDP sock");
            continue;
        }

        ssize_t res = sock_udp_send(&sock, message, sizeof(message), &server);
        if (res < 0) {
            puts("Error: unable to send message");
            sock_udp_close(&sock);
            continue;
        }

        puts("Message sent successfully");
        sock_udp_close(&sock);
        xtimer_sleep(5);
    }

    return 0;
}