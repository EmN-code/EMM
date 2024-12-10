#include <xtimer.h>
#include <net/gnrc/udp.h>
#include <net/af.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

char *state = "work"; // can change to "not work" as needed

int main(void) {
    uint8_t buf[128];
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);

    if (!netif) {
        printf("No network interface found\n");
        return 1;
    }

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR);

    while (1) {
        printf("Sending state: %s\n", state);
        snprintf((char *)buf, sizeof(buf), "state: %s", state);
        sock_udp_send(NULL, buf, strlen((char *)buf) + 1, &remote);
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}