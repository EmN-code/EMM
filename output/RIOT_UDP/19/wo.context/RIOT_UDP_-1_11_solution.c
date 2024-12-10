#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define MESSAGE "work"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t data[MAX_LLNADDR_SIZE]; // This buffer can be resized according to your needs
    ssize_t res;

    // convert IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: cannot parse IP");
        return 1;
    }

    printf("Sending UDP packets to %s:%u\n", SERVER_IP, SERVER_PORT);

    while (1) {
        res = sock_udp_send(NULL, MESSAGE, sizeof(MESSAGE), &server);

        if (res < 0) {
            puts("Error sending message");
        } else {
            printf("Sent: %s\n", MESSAGE);
        }

        xtimer_sleep(5);
    }
    return 0;
}