#include "net/sock/udp.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include <net/af.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    20001
#define SEND_INTERVAL  5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    char message[] = "work or not work";
  
    uint8_t addr_str[64];
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, SERVER_ADDRESS);
    ipv6_addr_to_str((char *)addr_str, &addr, 64);
    remote.addr.ipv6[0] = addr;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        return 1;
    }

    while (1) {
        sock_udp_send(&sock, message, sizeof(message), &remote);
        xtimer_sleep(SEND_INTERVAL);
    }
    return 0;
}