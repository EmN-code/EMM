#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5 * US_PER_SEC)  // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    int res;
    char *messages[2] = { "work", "not work" };
    uint8_t current_state = 0;

    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    while (1) {
        ssize_t bytes = sock_udp_send(&sock, messages[current_state], strlen(messages[current_state]), &remote);
        if (bytes < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("State '%s' sent to %s:%d\n", messages[current_state], SERVER_IP, SERVER_PORT);
        }

        current_state = !current_state;
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    return 0;
}