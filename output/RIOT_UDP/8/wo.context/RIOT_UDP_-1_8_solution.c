#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/inet.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

#define MESSAGE_WORK "Device is working"
#define MESSAGE_NOT_WORK "Device is not working"

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    // Convert IP address from string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("ERROR: Unable to parse IPv4 address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("ERROR: Unable to create UDP socket");
        return 1;
    }

    while (1) {
        const char *message = MESSAGE_WORK;  // Assume device is always working for this example
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            puts("ERROR: Unable to send message");
        } else {
            puts("Message sent");
        }

        xtimer_usleep(INTERVAL);
    }

    // Cleanup
    sock_udp_close(&sock);

    return 0;
}
