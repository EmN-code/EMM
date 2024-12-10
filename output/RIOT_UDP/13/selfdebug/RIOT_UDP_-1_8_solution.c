#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include "xtimer.h"
#include <stdio.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    20001
#define SEND_INTERVAL  (5U * US_PER_SEC)

static void send_udp_message(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *messages[2] = {"work", "not work"};
    static int toggle = 0;
    const char *msg = messages[toggle];
    ssize_t res;

    res = sock_udp_send(sock, msg, strlen(msg), remote);
    if (res < 0) {
        printf("Error sending UDP packet: %d\n", (int)res);
    } else {
        printf("UDP packet sent: %s\n", msg);
    }

    toggle = !toggle;  // Alternate between "work" and "not work"
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t server_addr[16];
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Parse server address
    if (inet_pton(AF_INET, SERVER_ADDRESS, &server_addr) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    memcpy(&server.addr.ipv6, server_addr, sizeof(server.addr.ipv6));

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    puts("UDP client initialized");

    // Periodically send UDP messages
    while (1) {
        send_udp_message(&sock, &server);
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}