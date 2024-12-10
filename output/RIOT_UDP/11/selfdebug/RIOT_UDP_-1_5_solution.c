#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    uint8_t addr[16];
    if (ipv6_addr_from_str((ipv6_addr_t *)addr, UDP_SERVER_IP) == NULL) {
        printf("Error: unable to parse IPv6 address\n");
        return -1;
    }
    memcpy(remote.addr.ipv6, addr, sizeof(addr));

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *state_working = "work";
    char *state_not_working = "not work";
    char *state = state_working;
    xtimer_ticks32_t last = xtimer_now();

    while (1) {
        sock_udp_send(&sock, state, strlen(state), &remote);
        printf("Sent device state: %s\n", state);

        // Swap state for demonstration purposes
        state = (state == state_working) ? state_not_working : state_working;

        // Wait for the send interval
        xtimer_periodic_wakeup(&last, SEND_INTERVAL);
    }

    return 0;
}
