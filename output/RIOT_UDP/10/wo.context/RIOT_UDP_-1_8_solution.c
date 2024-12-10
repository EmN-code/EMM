#include "xtimer.h"
#include "sock.h"
#include "net/ipv6/addr.h"
#include "stdio.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)  // Send every 5 seconds
#define SERVER_PORT    (20001)
#define SERVER_IP      "47.102.103.1"

int main(void) {
    /* create a UDP socket */
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char* msg = "Device state: work";

        ssize_t res = sock_udp_send(&sock, msg, strlen(msg), &server);
        if (res < 0) {
            printf("Could not send message\n");
        } else {
            printf("Message sent\n");
        }

        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }
    return 0;
}