#include "net/sock/udp.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/nib.h"
#include "net/gnrc/udp.h"
#include "msg.h"
#include "xtimer.h"
#include "random.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDRESS);

    while (1) {
        sock_udp_t sock;
        int res;
        char *state = random_uint32() % 2 == 0 ? "work" : "not work";
        
        res = sock_udp_create(&sock, NULL, NULL, 0);
        if (res != 0) {
            // Handle socket creation error
            continue;
        }

        ssize_t bytes_sent = sock_udp_send(&sock, state, strlen(state), &remote);
        if (bytes_sent < 0) {
            // Handle UDP send error
            continue;
        }

        sock_udp_close(&sock);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
