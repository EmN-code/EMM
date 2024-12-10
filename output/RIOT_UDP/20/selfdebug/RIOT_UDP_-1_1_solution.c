# INCLUDE GUARD
#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "xtimer.h"
#include "timex.h"
#include "net/udp.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    char state_work[] = "work";
    char state_not_work[] = "not work";
    bool device_working = true;  // Simulating the state, you can set it based on your condition

    // Init UDP
    sock_udp_create(&sock, &local, NULL, 0);
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);
    while (1) {
        if (device_working) {
            sock_udp_send(&sock, state_work, sizeof(state_work), &remote);
        }
        else {
            sock_udp_send(&sock, state_not_work, sizeof(state_not_work), &remote);
        }
        device_working = !device_working;  // Toggle state between work and not work for demonstration
        xtimer_sleep(INTERVAL);
    }
    return 0;
}

#endif /* UDP_CLIENT_H */