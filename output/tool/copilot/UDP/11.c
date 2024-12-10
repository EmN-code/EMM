#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "ztimer.h"

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     20001
#define SEND_INTERVAL   (5U * US_PER_SEC)  // 5 seconds

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static const char *device_state = "work";  // or "not work"

int main(void)
{
    // Resolve the server IP address
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP address");
        return 1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for the next interval
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}