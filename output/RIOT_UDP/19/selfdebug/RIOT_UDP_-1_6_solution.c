#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/protnum.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * US_PER_SEC) // in microseconds

static const char *STATE_WORK = "work";
static const char *STATE_NOT_WORK = "not work";

int main(void) {
    /* Define the remote server endpoint */
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    /* Convert server address string to binary form */
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address %s\n", SERVER_ADDR);
        return -1;
    }

    /* Initialize UDP socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    while (1) {
        /* Determine current state */
        const char *current_state = STATE_WORK;

        /* Send message to server */
        if (sock_udp_send(&sock, current_state, strlen(current_state), &remote) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("State '%s' sent to %s:%d\n", current_state, SERVER_ADDR, SERVER_PORT);
        }

        /* Wait for MESSAGE_INTERVAL microseconds */
        xtimer_usleep(MESSAGE_INTERVAL);
    }

    /* Close the UDP socket */
    sock_udp_close(&sock);

    return 0;
}