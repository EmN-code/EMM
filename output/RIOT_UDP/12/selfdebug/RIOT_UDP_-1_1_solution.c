#include <string.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"

static void send_udp_message(const char *server_addr, uint16_t server_port, const char *message) {
    sock_udp_ep_t server = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = server_port
    };

    if (inet_pton(AF_INET6, server_addr, &server.addr) != 1) {
        printf("Error: Unable to parse IPv6 address\n");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
        printf("Error: Unable to send message\n");
    } else {
        printf("Sent message: %s\n", message);
    }

    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        send_udp_message(SERVER_ADDR, SERVER_PORT, DEVICE_STATE);
        xtimer_sleep(5);
    }
    return 0;
}