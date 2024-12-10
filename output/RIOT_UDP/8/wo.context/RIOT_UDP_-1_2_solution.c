#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "xtimer.h"

#define SERVER_IPV4 "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IPV4, &server.addr.ipv4);
    
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char message[] = "Device state: work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, message, sizeof(message), &server);
        if (res < 0) {
            printf("Error sending: %d\n", res);
        } else {
            printf("Message sent to %s:%d\n", SERVER_IPV4, SERVER_PORT);
        }

        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
