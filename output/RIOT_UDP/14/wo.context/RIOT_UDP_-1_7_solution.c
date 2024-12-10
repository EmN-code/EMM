#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET,
                             .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *message = "work";
    while (1) {
        sock_udp_send(&sock, message, strlen(message), &server);
        printf("Sent message: %s\n", message);
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
        message = (strcmp(message, "work") == 0) ? "not work" : "work";
    }

    return 0;
}