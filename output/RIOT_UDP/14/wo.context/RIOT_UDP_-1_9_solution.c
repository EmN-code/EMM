#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

void send_state(sock_udp_ep_t *remote) {
    sock_udp_t sock;
    const char *state = "work";
    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        printf("Error creating UDP socket\n");
        return;
    }
    if (sock_udp_send(&sock, state, strlen(state), remote) < 0) {
        printf("Error sending data\n");
    }
    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid server address\n");
        return 1;
    }

    while (1) {
        send_state(&remote);
        xtimer_sleep(INTERVAL);
    }
    return 0;
}