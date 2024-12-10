#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include "net/af.h"
#include "net/protnum.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define STATE_MSG "work"
#define NOT_WORK_MSG "not work"
#define INTERVAL 5

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    if (inet_pton(AF_INET6, SERVER_ADDR, &server.addr.ipv6) <= 0) {
        puts("Error: invalid server address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock");
        return 1;
    }

    char *device_state = STATE_MSG;

    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
            puts("Error: failed to send UDP message");
        } else {
            printf("Sent state: %s\n", device_state);
        }

        ztimer_sleep(ZTIMER_MSEC, INTERVAL * MS_PER_SEC);
    }

    return 0;
}