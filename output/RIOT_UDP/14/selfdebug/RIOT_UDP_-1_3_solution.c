#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL (5U)

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void send_udp_message(sock_udp_t *sock) {
    const char *message = "work";
    if (sock_udp_send(sock, message, strlen(message), &server) < 0) {
        printf("Error sending UDP message\n");
    }
}

int main(void) {
    sock_udp_t sock;
    uint8_t addr[4];  // Use for storing IPv4 address

    if (inet_pton(AF_INET, SERVER_ADDR, &addr) != 1) {
        printf("Error: couldn’t convert IP address\n");
        return 1;
    }

    memcpy(&server.addr.ipv4, addr, sizeof(addr));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        send_udp_message(&sock);
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}