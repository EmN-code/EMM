#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    char data_work[] = "work";
    char data_not_work[] = "not work";
    char buf[128];
    int res;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    /* Convert the server IP address from string to a binary representation */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: could not parse IPv4 address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: could not create UDP sock");
        return 1;
    }

    xtimer_ticks32_t last_tick = xtimer_now();
    while (1) {
        /* Sending "work" status on each interval */
        res = sock_udp_send(&sock, data_work, strlen(data_work), &remote);
        if (res < 0) {
            puts("Error: unable to send data");
        }
        else {
            printf("Sent: %s\n", data_work);
        }

        /* Wait for next tick */
        xtimer_periodic_wakeup(&last_tick, INTERVAL);
    }

    return 0;
}