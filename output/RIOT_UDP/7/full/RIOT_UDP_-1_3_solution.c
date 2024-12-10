#include "sys/socket.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work or not work"
#define SLEEP_INTERVAL 5

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    int status = sock_udp_create(&sock, NULL, NULL, 0);
    if (status != 0) {
        printf("Failed to create UDP socket\n");
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote);
        if (res < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %s\n", MESSAGE);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}