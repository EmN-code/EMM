#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "ztimer.h"
#include "net/ipv4/addr.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
static sock_udp_t sock;

void send_device_state(void)
{
    const char *state = "work"; // or "not work"
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &server);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    } else {
        printf("Sent message: %s\n", state);
    }
}

int main(void)
{
    // Resolve server address
    ipv4_addr_t addr;
    if (ipv4_addr_from_str(&addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }
    memcpy(&server.addr.ipv4, &addr, sizeof(addr));

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    // Main loop
    while (1) {
        send_device_state();
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}