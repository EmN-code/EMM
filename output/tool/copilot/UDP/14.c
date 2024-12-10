#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "ztimer.h"

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     20001
#define SEND_INTERVAL   (5 * US_PER_SEC) // 5 seconds

static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
static sock_udp_t sock;

void send_device_state(void)
{
    const char *state = "work"; // Change to "not work" as needed
    ssize_t res;

    res = sock_udp_send(&sock, state, strlen(state), &server);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    } else {
        printf("Sent message: %s\n", state);
    }
}

int main(void)
{
    // Initialize server endpoint
    server.family = AF_INET;
    server.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP);

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_device_state();
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}