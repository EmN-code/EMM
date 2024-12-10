#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    sock_udp_t sock;
    char message[] = "Device State: Work";
    uint8_t server_addr[16];

    if (inet_pton(AF_INET, SERVER_IP, server_addr) == 0) {
        puts("Error: Malformed IP address");
        return -1;
    }

    memcpy(&remote.addr.ipv4, server_addr, sizeof(remote.addr.ipv4));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket");
        return -1;
    }

    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error: Message not sent");
        } else {
            puts("Message sent: Device State: Work");
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}