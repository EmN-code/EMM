#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "arpa/inet.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128

static const char *work_state = "work";
static const char *not_work_state = "not work";

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    char buffer[BUFFER_SIZE];

    /* Convert the server IPv6 address from string notation */
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return -1;
    }

    /* Create the UDP socket */
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error: unable to create socket\n");
        return -1;
    }

    while (1) {
        /* Choose work state */
        const char *state = (/* some condition */) ? work_state : not_work_state;

        /* Send state to server */
        ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
        if (res < 0) {
            printf("Error: unable to send data\n");
        }
        else {
            printf("Sent: %s\n", state);
        }

        /* Wait for 5 seconds */
        xtimer_sleep(5);
    }

    /* Close the UDP socket (unreachable) */
    sock_udp_close(&sock);

    return 0;
}