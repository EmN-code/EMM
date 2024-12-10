// Makefile snippet
USEMODULE += sock_udp
USEMODULE += xtimer

// main.c file
#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>  // Ensure the path is correct.
#include <net/af.h>
#include <net/protnum.h>

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = UDP_SERVER_PORT
};

static void sender_proc(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    char message[16];
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER_ADDRESS);

    // Initialize UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    // Send messages in a loop
    while (1) {
        snprintf(message, sizeof(message), "working");
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent");
        }
        xtimer_usleep(SEND_INTERVAL);
    }
}

int main(void) {
    puts("RIOT UDP client example");
    sender_proc();
    return 0;
}