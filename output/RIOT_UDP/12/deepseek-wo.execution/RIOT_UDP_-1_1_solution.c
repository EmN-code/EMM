
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV6_EP_ANY;

void send_state(const char *state)
{
    char buf[32];
    sprintf(buf, "Device state: %s", state);
    sock_udp_send(&sock, buf, strlen(buf), &server);
}

int main(void)
{
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = 12345;  // Local port to bind to
    server.port = SERVER_PORT;
    server.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL_MS);
        send_state("not work");
        xtimer_usleep(INTERVAL_MS);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
