#include <stdio.h>
#include <string.h>
#include <time.h>
#include "xtimer.h"
#include "shell.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buf[128];

    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    // Initialize UDP Socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to initialize socket");
        return 1;
    }

    while (1) {
        const char *status = "work";  // Change device state as necessary
        ssize_t res = sock_udp_send(&sock, status, strlen(status), &server);
        if (res < 0) {
            printf("Failed to send UDP packet:%zd\n", res);
        } else {
            printf("Device state ('%s') sent\n", status);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}