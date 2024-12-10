#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/inet/addr.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define INTERVAL (5U) // interval in seconds

int main(void) {
    sock_udp_ep_t remote;
    sock_udp_t sock;
    char* message = "work";

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    // Set the remote endpoint address family and port
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return -1;
    }

    // A loop to send messages every INTERVAL seconds
    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent to %s\n", SERVER_ADDR);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}