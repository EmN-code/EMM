#include <net/sock/udp.h>
#include <net/af.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    uint8_t addr[4];
    if (inet_pton(AF_INET, SERVER_IP, &addr) != 1) {
        puts("Error: Unable to parse IP address");
        return 1;
    }
    memcpy(&server.addr.ipv4, addr, sizeof(addr));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        puts("Error: Unable to create UDP sock");
        return 1;
    }

    char *working_state = "work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, working_state, strlen(working_state), &server);
        if (res >= 0) {
            puts("State sent successfully");
        } else {
            puts("Error sending state");
        }
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    return 0;
}