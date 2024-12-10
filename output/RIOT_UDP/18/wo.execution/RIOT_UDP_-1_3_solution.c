#include "net/emcute.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    char state[] = "work"; // or "not work"
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    while (1) {
        // Send UDP message
        ssize_t res = sock_udp_send(&sock, state, sizeof(state), &remote);

        if (res < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("Sent: %s\n", state);
        }

        xtimer_sleep(5);
    }

    // Close UDP socket
    sock_udp_close(&sock);

    return 0;
}