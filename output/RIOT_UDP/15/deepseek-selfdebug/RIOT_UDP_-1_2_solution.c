
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000U)

static char state_msg[] = "work";

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    int res;

    /* Initialize the UDP socket */
    res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    /* Convert server IP address from string to binary form */
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address: %d\n", res);
        return 1;
    }

    /* Main loop to send device state every 5 seconds */
    while (1) {
        res = sock_udp_send(&sock, state_msg, strlen(state_msg), &remote);
        if (res < 0) {
            printf("Error sending UDP packet: %d\n", res);
        } else {
            printf("Sent device state: %s\n", state_msg);
        }

        xtimer_sleep(INTERVAL_MS / 1000);
    }

    /* Close the UDP socket */
    sock_udp_close(&sock);

    return 0;
}
