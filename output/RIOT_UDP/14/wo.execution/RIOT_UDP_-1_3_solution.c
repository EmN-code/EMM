#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "netinet/in.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work or not work"
#define WAIT_INTERVAL 5

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server_ep = { .family = AF_INET6, .port = SERVER_PORT };
    char server_addr_str[] = SERVER_IP;
    ipv6_addr_t server_addr;

    if (!ipv6_addr_from_str(&server_addr, server_addr_str)) {
        return -1;  // Handle error
    }
    memcpy(&server_ep.addr.ipv6, &server_addr, sizeof(server_addr));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        return -1;  // Handle error
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server_ep) < 0) {
            return -1;  // Handle error
        }

        xtimer_sleep(WAIT_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}