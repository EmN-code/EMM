#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_PORT 20001
#define UDP_SEND_INTERVAL (5U * US_PER_SEC)
#define SERVER_IP "47.102.103.1"

static char message[] = "Device Status: work";

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT};

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: Unable to parse server address");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            puts("Error: Unable to send message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}