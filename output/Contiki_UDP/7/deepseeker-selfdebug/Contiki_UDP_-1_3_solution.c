
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/netif.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV6_EP_ANY;

void send_device_state(void) {
    char buf[] = DEVICE_STATE_MSG;
    if (sock_udp_send(&sock, buf, strlen(buf), &server) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("Device state sent");
    }
}

int main(void) {
    // Initialize UDP socket
    server.port = UDP_SERVER_PORT;
    server.family = AF_INET;
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, UDP_SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
