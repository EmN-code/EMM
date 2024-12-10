#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "ztimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * MS_PER_SEC)

static const char *device_state = "work"; // or "not work"

static void send_device_state(sock_udp_ep_t *remote)
{
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, device_state, strlen(device_state), remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("Device state sent");
    }

    sock_udp_close(&sock);
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_IP);

    while (1) {
        send_device_state(&remote);
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL);
    }

    return 0;
}