#include <ztimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <mutex.h>

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    20001
#define MESSAGE        "work or not work"
#define INTERVAL       (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting server address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (true) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent successfully");
        }
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }
    
    sock_udp_close(&sock);
    return 0;
}