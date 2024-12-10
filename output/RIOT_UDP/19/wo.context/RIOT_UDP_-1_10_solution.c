#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "Device state: work"
#define INTERVAL (5U * US_PER_SEC)

typedef struct {
    char addr_str[16];
    uint16_t port;
} udp_endpoint_t;

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buf[128];

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* open UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Could not create UDP socket");
        return 1;
    }

    while (1) {
        printf("Sending message: %s\n", MESSAGE);

        if (sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote) < 0) {
            printf("Error sending UDP packet\n");
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}