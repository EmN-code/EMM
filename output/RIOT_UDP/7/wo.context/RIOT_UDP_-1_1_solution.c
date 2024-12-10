#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define UDP_DATA_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    char *state_msg[] = {"work", "not work"};
    int state = 0;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return 1;
    }

    if (sock_udp_str2ep(&remote, SERVER_ADDR) < 0) {
        printf("Error: Unable to parse server address\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, state_msg[state], strlen(state_msg[state]), &remote) < 0) {
            printf("Error: Failed to send UDP message\n");
        } else {
            printf("Sent state: %s\n", state_msg[state]);
        }

        state = !state;
        xtimer_sleep(5);
    }

    // Close socket
    sock_udp_close(&sock);
    return 0;
}