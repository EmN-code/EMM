#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "ztimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * US_PER_SEC) // 5 seconds

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static const char *device_state = "work"; // or "not work"

void send_device_state(sock_udp_t *sock)
{
    ssize_t res = sock_udp_send(sock, device_state, strlen(device_state), &server);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    } else {
        printf("Sent device state: %s\n", device_state);
    }
}

int main(void)
{
    sock_udp_t sock;
    int res;

    // Resolve server address
    res = ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_ADDR);
    if (res == 0) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    // Create UDP socket
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_device_state(&sock);
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}