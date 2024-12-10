#include <xtimer.h>
#include <udp.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote;
    char *message;
    ssize_t res;

    remote.family = AF_INET6;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR);

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        // Example state message
        message = "work";

        res = sock_udp_send(&sock, message, strlen(message), &remote);

        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}