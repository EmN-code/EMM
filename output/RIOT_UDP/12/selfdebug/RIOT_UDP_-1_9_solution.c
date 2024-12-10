#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define TIMER_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    char *state = "work";
    while (1) {
        if (sock_udp_send(NULL, state, strlen(state), &remote) < 0) {
            printf("Failed to send UDP message\n");
        }
        printf("State sent: %s\n", state);
        xtimer_sleep(TIMER_INTERVAL);
    }
    return 0;
}