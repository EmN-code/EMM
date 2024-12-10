#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/af.h>

#define SLEEP                        (5U)
#define SERVER_PORT                  20001
#define SERVER_ADDR                  "47.102.103.1"
#define BUF_SIZE                     64

static sock_udp_ep_t remote;
static char buf[BUF_SIZE];

int main(void) {
    int res;
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;

    res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error converting IP address\n");
        return 1;
    }

    char work_state[] = "work";
    char not_work_state[] = "not work";

    while (1) {
        snprintf(buf, sizeof(buf), "Device state: %s", work_state);

        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent: %s\n", buf);
        }

        xtimer_sleep(SLEEP);
    }

    return 0;
}