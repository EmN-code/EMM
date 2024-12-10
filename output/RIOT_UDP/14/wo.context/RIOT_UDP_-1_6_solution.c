#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <stdio.h>

#define IPV4_ADDRESS "47.102.103.1"
#define PORT 20001
#define MESSAGE "work"
#define SEND_INTERVAL (5U * 1000000U) // send every 5 seconds converted to microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = PORT };
    uint32_t sleep_time = SEND_INTERVAL;
    uint8_t addr[NETADDR_MAXLEN];
    xtimer_ticks32_t last_wakeup = xtimer_now();

    printf("Starting UDP client\n");

    // Convert the IP address string to an actual IP address
    if (ipv4_addr_from_str((ipv4_addr_t *)addr, IPV4_ADDRESS) == NULL) {
        printf("Error: unable to parse remote address\n");
        return 1;
    }

    remote.addr.ipv4 = *(ipv4_addr_t *)addr;

    while (1) {
        ssize_t res = sock_udp_send(NULL, MESSAGE, sizeof(MESSAGE), &remote);
        if (res < 0) {
            printf("Error sending packet: %d\n", (int)res);
        } else {
            printf("Packet sent\n");
        }
        xtimer_periodic_wakeup(&last_wakeup, sleep_time);
    }
    return 0;
}
