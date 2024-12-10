
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .port = SERVER_PORT, .family = AF_INET };
static char buffer[16];

void send_device_state(const char *state) {
    sock_udp_ep_t remote = server;
    ipv4_addr_t server_addr;

    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return;
    }
    remote.addr.ipv4 = server_addr;

    strncpy(buffer, state, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (sock_udp_send(NULL, buffer, strlen(buffer), &remote) < 0) {
        puts("Error: failed to send UDP packet");
    } else {
        printf("Sent: %s\n", buffer);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_device_state(DEVICE_STATE_WORK);
        xtimer_usleep_until(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}
