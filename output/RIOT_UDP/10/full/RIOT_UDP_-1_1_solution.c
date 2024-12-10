#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gnrc/netif.h>
#include <net/sock_udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define RECONNECT_INTERVAL (1U)
#define MESSAGE_INTERVAL (5U)

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    server.family = AF_INET;

    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: Unable to parse server IP address.");
        return 1;
    }

    char *message = "device state: work";
    char buffer[128];
    int retries = 0;

    while (1) {
        int rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc != 0) {
            printf("Failed to create socket: %d\n", rc);
            xtimer_sleep(RECONNECT_INTERVAL);
            continue;
        }

        int n = snprintf(buffer, sizeof(buffer), "%s", message);

        rc = sock_udp_send(&sock, buffer, n, &server);
        if (rc < 0) {
            printf("Failed to send message: %d\n", rc);
            sock_udp_close(&sock);
            xtimer_sleep(RECONNECT_INTERVAL);
            continue;
        }

        printf("Sent message: %s\n", message);
        sock_udp_close(&sock);

        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}