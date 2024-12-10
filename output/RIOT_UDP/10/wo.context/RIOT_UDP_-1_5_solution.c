#include "xtimer.h"
#include "net/sock/udp.h"
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET,
                             .netif = SOCK_ADDR_ANY_NETIF,
                             .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr);

    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP sock: %d\n", res);
        return 1;
    }

    char *message = "work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            printf("Error sending message: %d\n", (int)res);
        } else {
            printf("Sent message: %s\n", message);
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}