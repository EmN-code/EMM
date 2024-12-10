#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <stdio.h>

#define UDP_PORT 20001
#define SERVER_IP "47.102.103.1"

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = UDP_PORT
};

char message_work[] = "work";
char message_not_work[] = "not work";

int main(void) {
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    char *msg = message_work;
    while (1) {
        sock_udp_send(&sock, msg, strlen(msg), &server);
        puts("Message sent");
        xtimer_sleep(5);

        // Toggle message
        if (strcmp(msg, message_work) == 0) {
            msg = message_not_work;
        } else {
            msg = message_work;
        }
    }

    // Close socket (not reachable in this example)
    sock_udp_close(&sock);

    return 0;
}