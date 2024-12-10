#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server IP address");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return -1;
    }

    const char *state_msg = "Device state: work";

    while (1) {
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent!");
        }

        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}