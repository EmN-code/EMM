#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .addr.ipv4 = { 0 },
        .port = SERVER_PORT
    };

    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        ssize_t res = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server);
        if (res < 0) {
            printf("Error sending data\n");
        } else {
            printf("Sent: %s\n", MESSAGE);
        }
        xtimer_sleep(5);
    }
    return 0;
}