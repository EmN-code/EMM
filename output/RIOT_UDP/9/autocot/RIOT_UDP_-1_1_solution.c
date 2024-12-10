#include <xtimer.h>
#include "net/af.h"
#include "net/gnrc.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    char *dev_state = "Device state: Work";
    uint16_t msg_counter = 0;

    /* Resolve the address of the server */
    if (gnrc_netif_ipv6_addr_from_str(SERVER_IP, &server.addr.ipv6) == NULL) {
        puts("Error: Could not resolve the server address.");
        return -1;
    }

    while (1) {
        sock_udp_t sock;
        int res;

        /* Open UDP connection */
        if ((res = sock_udp_create(&sock, NULL, NULL, 0)) < 0) {
            printf("Error: unable to create UDP sock. %d\n", res);
            return -1;
        }

        /* Send message to server */
        printf("Sending message %u: %s\n", msg_counter++, dev_state);
        if ((res = sock_udp_send(&sock, dev_state, strlen(dev_state), &server)) < 0) {
            printf("Error: unable to send UDP message. %d\n", res);
        }

        /* Close socket */
        sock_udp_close(&sock);

        /* Wait for the next iteration */
        xtimer_sleep(INTERVAL);
    }

    return 0;
}