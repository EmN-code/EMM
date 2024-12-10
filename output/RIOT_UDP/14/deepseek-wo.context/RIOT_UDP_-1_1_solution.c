
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;

void send_state(const char *state)
{
    char buf[32];
    sprintf(buf, "Device state: %s", state);
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void)
{
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = 12345;  // Local port to bind to
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
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
