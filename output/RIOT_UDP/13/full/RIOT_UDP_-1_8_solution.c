#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include <net/sock/udp.h>
#include <errno.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL 5

// Declare as a prototype
int inet_pton(int af, const char *src, void *dst);

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[128];

    // Convert IP address from text to binary form
    while (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address, retrying in 1 second.\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    sock_udp_t sock;

    // Create UDP socket
    while (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Failed to create UDP socket, retrying in 1 second.\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    printf("UDP connection established.
");

    // Send device state every INTERVAL seconds
    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            printf("Error sending message, retrying.\n");
        } else {
            printf("Message sent\n");
        }
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}