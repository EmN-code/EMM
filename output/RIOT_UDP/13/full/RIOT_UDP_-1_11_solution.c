#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "udp.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "net/sock/udp.h"

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microsecond units

static void send_udp_message(sock_udp_ep_t *remote, const char *message) {
    sock_udp_t sock;
    char response[64];
    size_t len;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), remote) < 0) {
            puts("Error sending UDP data");
        } else {
            puts("UDP message sent");
        }

        len = sock_udp_recv(&sock, response, sizeof(response) - 1, 1 * US_PER_SEC, NULL);
        if (len >= 0) {
            response[len] = '\0';
            printf("Received: %s\n", response);
        }

        ztimer_sleep(ZTIMER_MSEC, MESSAGE_INTERVAL);
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = UDP_SERVER_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER_ADDRESS) == NULL) {
        puts("Error parsing server address");
        return -1;
    }

    while(1) {
        send_udp_message(&remote, "work");
        ztimer_sleep(ZTIMER_MSEC, MESSAGE_INTERVAL);
    }

    return 0;
}