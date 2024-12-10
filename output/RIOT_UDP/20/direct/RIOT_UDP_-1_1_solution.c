#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = UDP_SERVER_PORT;

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER_IP) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    const char *device_state = "work";

    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            puts("Error: unable to send UDP packet");
        } else {
            puts("Device state sent successfully");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}