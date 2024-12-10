#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC) // Sending interval in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_t addr;

    // Convert IP address from string format to byte format using inet_pton
    if (inet_pton(AF_INET, SERVER_ADDR, &addr) != 1) {
        puts("Error: unable to convert server address");
        return 1;
    }

    memcpy(&remote.addr.ipv6, &addr, sizeof(addr));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    const char *message = "Device state: work";
    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}