#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_SEC 5
#define DEVICE_STATE "work"

void udp_send_message(void) {
    sock_udp_ep_t remote = {};
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        puts("Error creating UDP socket");
        return;
    }

    while (1) {
        if (sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(INTERVAL_SEC);
    }
}

int main(void) {
    puts("RIOT UDP client example");
    udp_send_message();
    return 0;
}