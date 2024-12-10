#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SERVER_PORT (20001U)
#define INTERVAL (5U * US_PER_SEC)

static const char *server_ip = "47.102.103.1";

static void udp_send(sock_udp_ep_t *remote) {
    char *message = "work or not work";
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), remote) < 0) {
        puts("Error sending message");
    } else {
        printf("Sent: %s\n", message);
    }

    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT
    };

    inet_pton(AF_INET, server_ip, &remote.addr.ipv4);

    while (1) {
        udp_send(&remote);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}