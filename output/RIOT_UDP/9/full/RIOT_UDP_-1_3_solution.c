#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>

#define SERVER_ADDR      "47.102.103.1"
#define SERVER_PORT      20001
#define BUF_SIZE         128
#define SEND_INTERVAL    5

static void send_device_state(sock_udp_ep_t *remote) {
    /* Define the state message */
    const char *state_msg = "work or not work";

    /* Create a socket and bind to it */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    while (1) {
        /* Send the state message to the server */
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), remote) < 0) {
            printf("Error sending message\n");
            sock_udp_close(&sock);
            return;
        }
        printf("Message sent\n");

        /* Sleep for SEND_INTERVAL seconds */
        xtimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
}

int main(void) {
    /* Set up the remote endpoint */
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR) == NULL) {
        printf("Error setting server address\n");
        return 1;
    }

    /* Validate the remote address */
    if (!remote.addr.ipv4.u8) {
        printf("Invalid IPv4 address\n");
        return 1;
    }

    printf("UDP client setup complete\n");

    /* Send device state periodically */
    send_device_state(&remote);

    return 0;
}