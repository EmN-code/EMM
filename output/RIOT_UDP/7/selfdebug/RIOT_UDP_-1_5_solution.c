#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}