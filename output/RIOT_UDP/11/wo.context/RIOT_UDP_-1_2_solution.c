#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_PORT (20001)
#define CLIENT_MSG_WORK "work"
#define CLIENT_MSG_NOT_WORK "not work"
#define SEND_INTERVAL (5U) // in seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, "47.102.103.1", &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    while (1) {
        const char *message = CLIENT_MSG_WORK; // Change to CLIENT_MSG_NOT_WORK as needed

        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("Message '%s' sent\n", message);
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}