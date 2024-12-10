
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
static char state_msg[20];

void send_state(void) {
    static int state = 0;
    state = !state;  // Toggle state

    if (state) {
        strcpy(state_msg, "work");
    } else {
        strcpy(state_msg, "not work");
    }

    sock_udp_ep_t remote = server;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4.u32);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);
    sock_udp_send(&sock, state_msg, strlen(state_msg), NULL);
    sock_udp_close(&sock);
}

int main(void) {
    printf("Starting UDP client...\n");

    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
