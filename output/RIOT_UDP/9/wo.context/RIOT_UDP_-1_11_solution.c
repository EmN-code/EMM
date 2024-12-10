#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include <string.h>

#define SERVER_PORT 20001
#define SERVER_IPV4 "47.102.103.1"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote;
    sock_udp_t sock;
    char message[] = "work";
    unsigned char addr[sizeof(sock_udp_ep_t::addr.ipv4)];

    int res = inet_pton(AF_INET, SERVER_IPV4, &addr);
    if (res != 1) {
        puts("inet_pton failed");
        return 1;
    }

    // Setup the remote address
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    memcpy(remote.addr.ipv4, addr, sizeof(addr));

    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        puts("Error creating UDP sock");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            puts("Error sending message");
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }
    return 0;
}