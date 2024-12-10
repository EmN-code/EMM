#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <net/ipv6/addr.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5

int main(void) {
    sock_udp_ep_t server;
    server.family = AF_INET;
    server.port = SERVER_PORT;
    
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error: unable to parse server IP\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    const char *msg = "work";

    while (1) {
        if (sock_udp_send(&sock, msg, strlen(msg), &server) < 0) {
            printf("Error: unable to send message\n");
        }
        printf("State sent: %s\n", msg);

        xtimer_sleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}