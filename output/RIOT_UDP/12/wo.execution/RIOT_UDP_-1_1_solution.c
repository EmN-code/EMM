#include "stdio.h"
#include "xtimer.h"
#include "net/af.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL_S (5U)

static char device_state[] = "work or not work";

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .addr.ipv6 = {0}, .port = SERVER_PORT };
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;

    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, device_state, sizeof(device_state), &server) < 0) {
            puts("Error: Unable to send data\n");
        } else {
            puts("Data sent successfully\n");
        }
        xtimer_sleep(SEND_INTERVAL_S);
    }

    return 0;
}