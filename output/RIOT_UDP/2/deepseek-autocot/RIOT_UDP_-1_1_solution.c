
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static sock_udp_ep_t server = { .port = SERVER_PORT, .family = AF_INET };
static char buf[64];

void send_device_state(void)
{
    int res;
    sock_udp_t sock;
    sock_udp_ep_t remote = server;

    /* Resolve server IP address */
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP address");
        return;
    }

    /* Initialize UDP socket */
    res = sock_udp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    /* Send device state message */
    strcpy(buf, DEVICE_STATE_MSG);
    res = sock_udp_send(&sock, buf, strlen(buf), NULL);
    if (res < 0) {
        puts("Error: unable to send UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    /* Close UDP socket */
    sock_udp_close(&sock);
}

int main(void)
{
    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
