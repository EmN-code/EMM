#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001

static sock_udp_ep_t udp_server = {
    .family = AF_INET,
    .port = UDP_SERVER_PORT
};

int main(void) {
    xtimer_ticks32_t last = xtimer_now();
    int res = inet_pton(AF_INET, UDP_SERVER_ADDR, &udp_server.addr.ipv4);
    if (res <= 0) {
        printf("Error: invalid IP address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    char *message;
    while (1) {
        message = "Device state: work";  // Change to "not work" as needed
        if (sock_udp_send(&sock, message, strlen(message), &udp_server) < 0) {
            printf("Error: unable to send UDP packet\n");
        }
        xtimer_periodic_wakeup(&last, 5 * US_PER_SEC);
    }
    return 0;
}