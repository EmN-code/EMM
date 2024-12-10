#include <xtimer.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return -1;
    }

    const char *state_msg = "work"; // Change this to "not work" as needed

    while (1) {
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            printf("Sent message: %s\n", state_msg);
        }

        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}