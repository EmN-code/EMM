
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/ipv6/udp-socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL 5  // in seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str((inet_addr_t *)&remote.addr.ipv4, SERVER_IP);

    char buffer[20];
    strncpy(buffer, DEVICE_STATE, sizeof(buffer));

    while (1) {
        sock_udp_send(NULL, buffer, strlen(buffer), &remote);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
