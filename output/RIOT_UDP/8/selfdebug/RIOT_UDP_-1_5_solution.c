#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/sock/udp.h" 
#include "net/ipv4/addr.h" 
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static void die(const char *msg) {
    puts(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t addr[sizeof(remote.addr.ipv4)];

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        die("inet_pton() failed");
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        die("sock_udp_create() failed");
    }

    const char *message_working = "work";
    const char *message_not_working = "not work";
    const char *current_state = message_working; // Change this variable accordingly

    while (1) {
        ssize_t res = sock_udp_send(&sock, current_state, strlen(current_state), &remote);
        if (res < 0) {
            die("Sending failed");
        }
        xtimer_sleep(5);
    }

    return 0;
}