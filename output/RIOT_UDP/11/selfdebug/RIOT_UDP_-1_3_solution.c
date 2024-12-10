#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .port = UDP_SERVER_PORT };
    int res;
    char message[] = "work";

    // Convert string IP address to binary form
    res = inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        if (res == 0) {
            printf("inet_pton: Invalid network address\n");
        } else {
            perror("inet_pton");
        }
        return 1;
    }

    while (1) {
        // Send the message to the UDP server
        if (sock_udp_send(NULL, message, sizeof(message), &remote) < 0) {
            printf("Error sending UDP packet\n");
            return 1;
        }
        printf("Message sent: %s\n", message);

        // Wait for given interval
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}