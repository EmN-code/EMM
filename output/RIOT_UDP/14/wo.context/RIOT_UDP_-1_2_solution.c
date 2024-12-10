#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 64
#define SEND_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buf[BUF_SIZE];
    int res;

    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Failed to parse server address\n");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        const char *msg = "Device state: work";
        memcpy(buf, msg, strlen(msg));

        res = sock_udp_send(&sock, buf, strlen(msg), &server);

        if (res < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("Sent message: %s\n", msg);
        }

        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}