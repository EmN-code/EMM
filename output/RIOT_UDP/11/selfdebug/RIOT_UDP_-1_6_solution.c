#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_IP) == NULL) {
        puts("Error: unable to parse server address");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return -1;
    }

    char *state_msg = "work";
    while (1) {
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent!");
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}