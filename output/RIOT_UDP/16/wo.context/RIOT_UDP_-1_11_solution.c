#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include <string.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define SENDING_INTERVAL (5 * US_PER_SEC)  // Send every 5 seconds

int main(void) {
    /* Set up the remote server address */
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert server address.");
        return 1;
    }

    /* Open a UDP sock */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP sock\n");
        return 1;
    }

    /* Create a periodic timer */
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        const char *msg = "work";  // or "not work" based on the actual device status

        /* Send the message to the server */
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            printf("Error: Could not send message\n");
        }
        else {
            printf("Message sent\n");
        }

        /* Wait for the next interval */
        xtimer_periodic_wakeup(&last_wakeup, SENDING_INTERVAL);
    }

    return 0;  // Should never be reached
}