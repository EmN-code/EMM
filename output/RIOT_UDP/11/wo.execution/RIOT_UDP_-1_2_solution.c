#include "netinet/in.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U) /* in seconds */

int main(void)
{
    int res;
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .ipv4 = { 0 }, .port = SERVER_PORT };

    /* converting IP address from text to binary form */
    inet_pton(AF_INET, SERVER_IP, &remote.ipv4);

    /* create the UDP socket */
    res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        return -1; /* handle error */
    }

    while (1) {
        const char *msg = "work or not work";
        res = sock_udp_send(&sock, msg, sizeof(msg), &remote);
        if (res < 0) {
            /* handle error */
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}