#include "udp.h"
#include "timex.h"
#include "thread.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5
#define DEVICE_STATE "work"

static void send_device_state(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDRESS);
    remote.port = SERVER_PORT;

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        sock_udp_send(&sock, DEVICE_STATE, sizeof(DEVICE_STATE), &remote);
        xtimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
}

int main(void) {
    send_device_state();
    return 0;
}