
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .port = SERVER_PORT, .family = AF_INET };
static char buf[64];

void send_device_state(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    strcpy(buf, DEVICE_STATE_MSG);
    if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
