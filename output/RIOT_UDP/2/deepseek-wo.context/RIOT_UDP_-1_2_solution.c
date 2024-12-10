
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    char buf[] = DEVICE_STATE;
    int result;

    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    result = sock_udp_create(&sock, NULL, &remote, 0);
    if (result < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        result = sock_udp_send(&sock, buf, strlen(buf), &remote);
        if (result < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent device state: %s\n", buf);
        }

        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
