#include <net/sock/udp.h>
#include <net/inet_addr.h>
#include <xtimer.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = {.family = AF_INET, .port = SERVER_PORT};

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: unable to parse remote address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    while (1) {
        const char *message = "work";
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}