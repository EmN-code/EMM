
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_t sock;
static sock_udp_ep_t remote;

static void _send_state(void)
{
    const char *state = DEVICE_STATE;
    size_t state_len = strlen(state);

    if (sock_udp_send(&sock, state, state_len, &remote) < 0) {
        puts("Error sending state");
    } else {
        puts("State sent successfully");
    }
}

int main(void)
{
    /* Initialize the remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    /* Convert IP address from string to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4.u32) != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    /* Create the UDP socket */
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        _send_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    /* Close the socket */
    sock_udp_close(&sock);

    return 0;
}
