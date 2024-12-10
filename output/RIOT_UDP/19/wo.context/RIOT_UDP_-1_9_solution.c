#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_PORT 20001
#define CLIENT_MSG_INTERVAL (5U * US_PER_SEC)
#define SERVER_ADDR "47.102.103.1"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char message[] = "Device Status: work or not work";
    int res;

    printf("Starting UDP client\n");

    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        puts("Error: Unable to parse server address");
        return 1;
    }

    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        puts("Error: Creating UDP socket");
        return 1;
    }

    while (1) {
        res = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (res < 0) {
            puts("Error: Sending message");
        } else {
            puts("Message sent");
        }

        xtimer_sleep(CLIENT_MSG_INTERVAL);
    }

    return 0;
}