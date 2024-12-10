#include <net/sock/udp.h>
#include <net/sock.h>
#include <inet_pton.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = 20001
    };

    if(inet_pton(AF_INET, "47.102.103.1", &server.addr.ipv4) != 1) {
        puts("Failed to convert IP address");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Failed to create UDP socket");
        return -1;
    }

    char *state = "work";
    while (1) {
        if (sock_udp_send(&sock, state, strlen(state), &server) < 0) {
            puts("Failed to send message");
            return -1;
        }
        printf("Sent device state: %s\n", state);
        xtimer_sleep(5);
    }
    return 0;
}