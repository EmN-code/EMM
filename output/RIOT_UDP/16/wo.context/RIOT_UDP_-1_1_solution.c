#include <string.h>
#include "xtimer.h"
#include "net/udp.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

#define SEND_INTERVAL (5U * US_PER_SEC)

static void _send_udp_state(sock_udp_ep_t *remote)
{
    const char *state = STATE_WORK; // Example state, modify as needed
    int res = sock_udp_send(NULL, state, strlen(state), remote);
    if (res < 0) {
        printf("Error sending UDP packet\n");
    }
    else {
        printf("Sent UDP packet: %s\n", state);
    }
}

int main(void)
{
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    if (inet_pton(AF_INET, SERVER_IP, &server.addr) <= 0) {
        puts("Error: unable to parse server IP\n");
        return 1;
    }

    while (1) {
        _send_udp_state(&server);
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}