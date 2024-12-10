#include "net/sock/udp.h"
#include "stdlib.h"
#include "msg.h"
#include "net/sock/util.h"
#include "net/af.h"
#include "dht.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "ztimer.h"
#include "paho_mqtt.h"
#include "bool.h"

#define UDP_SERVER "47.102.103.1"
#define UDP_PORT 20001

static sock_udp_ep_t server;

int main(void) {
    sock_udp_t sock;
    msg_t msg_queue[8];

    /* Initialize the RIOT message queue */
    msg_init_queue(msg_queue, 8);

    /* Set up the UDP server endpoint */
    if (!sock_udp_str2ep(&server, "47.102.103.1:20001")) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    /* Create the UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        const char *state = "work";

        /* Send the state to the UDP server */
        if (sock_udp_send(&sock, state, strlen(state), &server) < 0) {
            printf("Error: unable to send data\n");
        } else {
            printf("Sent state: %s\n", state);
        }

        /* Wait for 5 seconds */
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    /* Close the socket (not reachable in this loop) */
    sock_udp_close(&sock);
    return 0;
}