#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gnrc/netif.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5    // interval for sending the message in seconds

static gnrc_netif_t *netif_get_any(void) {
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    return (netif != NULL) ? netif : NULL;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6 };
    char *message = "Device state: work";
    int res;

    // Find and configure a network interface
    gnrc_netif_t *netif = netif_get_any();
    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    remote.port = SERVER_PORT;
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }

    sock_udp_t sock;
    if ((res = sock_udp_create(&sock, NULL, NULL, 0)) < 0) {
        printf("Error: unable to create UDP sock: %s\n", sock_strerror(res));
        return 1;
    }

    // Infinite loop to send the message every INTERVAL seconds
    while (1) {
        if ((res = sock_udp_send(&sock, message, strlen(message), &remote)) < 0) {
            printf("Error: could not send: %s\n", sock_strerror(res));
        } else {
            printf("Sent message: %s\n", message);
        }
        xtimer_sleep(INTERVAL);
    }

    // Close socket
    sock_udp_close(&sock);
    return 0;
}