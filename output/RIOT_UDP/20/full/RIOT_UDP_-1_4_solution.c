#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) == 0) {
        printf("Error: unable to parse server address\n");
        return -1;
    }

    sock_udp_t sock;
    sock_udp_ep_t local = {
        .family = AF_INET,
        .port = LOCAL_PORT
    };

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return -1;
    }

    char message[] = "work";

    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error: unable to send message\n");
        }
        else {
            printf("Message sent: %s\n", message);
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}