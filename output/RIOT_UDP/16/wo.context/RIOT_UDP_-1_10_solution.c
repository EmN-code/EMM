#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     20001
#define SEND_INTERVAL   5 * US_PER_SEC

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    char *message = "work";  // Message indicating device state
    size_t msg_len = strlen(message);

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return -1;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock");
        return -1;
    }

    while (1) {
        if (sock_udp_send(&sock, message, msg_len, &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}