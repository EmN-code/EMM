#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = 20001 };

    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse UDP server address");
        return 1;
    }
    static sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP sock: %d\n", res);
        return 1;
    }

    char msg[] = "work";

    while (1) {
        printf("Sending message to %s:%u\n", "47.102.103.1", remote.port);
        ssize_t size = sock_udp_send(&sock, msg, sizeof(msg), &remote);
        if (size < 0) {
            printf("Error sending UDP packet: %d\n", (int)size);
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}