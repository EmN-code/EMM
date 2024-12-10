#include <string.h>
#include <stdio.h>
#include <net/inet.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

static const char* msg = "work"; // message to send

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    int res = inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    if (res <= 0) {
        if (res == 0) {
            puts("Invalid IP address format");
        } else {
            puts("inet_pton() failed");
        }
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        ssize_t result = sock_udp_send(&sock, msg, strlen(msg), &server);
        if (result < 0) {
            printf("Error sending UDP packet: %d\n", result);
        } else {
            printf("Sent: %s\n", msg);
        }

        xtimer_sleep(5);
    }

    // Normally you won't reach here
    return 0;
}