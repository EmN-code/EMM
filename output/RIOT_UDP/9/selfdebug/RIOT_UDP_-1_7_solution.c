#include <xtimer.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/gnrc/udp.h>

#define UDP_SERVER "47.102.103.1"
#define UDP_PORT   20001
#define TIMER_DELAY 5U

static char device_state_work[] = "work";
static char device_state_not_work[] = "not work";

int main(void)
{
    uint8_t local_addr[16];
    inet_pton(AF_INET6, "::", local_addr);  // Set to unspecified address for binding
    sock_udp_ep_t remote = { .family = AF_INET6, .port = UDP_PORT };

    if (inet_pton(AF_INET6, UDP_SERVER, &remote.addr) == 0) {
        printf("Error: unable to parse server IP address\n");
        return 1;
    }

    sock_udp_t sock;
    sock_udp_create(&sock, (sock_udp_ep_t){ .family = AF_INET6 }, NULL, 0);

    while (1) {
        sock_udp_send(&sock, device_state_work, strlen(device_state_work), &remote);
        xtimer_sleep(TIMER_DELAY);
    }

    return 0;
}