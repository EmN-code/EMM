#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/gnrc/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_PERIOD (5 * US_PER_SEC)

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    gnrc_netif_t *netif = gnrc_netif_iter(NULL); // Obtain network interface
    if (!netif) {
        printf("Error: No network interface found\n");
        return 1;
    }

    sock_udp_ep_t remote = { .family = AF_INET6, .netif = netif->pid };
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr) != 1) {
        printf("Error: Invalid server address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return 1;
    }

    const char *state = "Device: work"; // Fixed message for demonstration
    while (1) {
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            printf("Error: Could not send UDP packet\n");
        } else {
            printf("State sent: '%s'\n", state);
        }
        xtimer_sleep(5); // Sleep for the define period
    }

    return 0;
}