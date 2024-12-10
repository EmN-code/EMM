#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define TIME_INTERVAL 5

int main(void) {
    sock_udp_ep_t remote;
    sock_udp_t sock;
    char message[] = "work";
    int rc; 

    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP) == NULL) {
        puts("Error converting server address");
        return 1;
    }

    while (1) {
        rc = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (rc < 0) {
            printf("Failed to send message, retrying in 1 second\n");
            xtimer_sleep(1);
        } else {
            printf("Sent message: %s\n", message);
            xtimer_sleep(TIME_INTERVAL);
        }
    }

    sock_udp_close(&sock);
    return 0;
}