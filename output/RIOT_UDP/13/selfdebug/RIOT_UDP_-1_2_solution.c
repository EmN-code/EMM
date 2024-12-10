#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT   20001
#define MESSAGE_INTERVAL (5 * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *message = "work";
    size_t message_len = strlen(message);

    while (1) {
        sock_udp_send(&sock, message, message_len, &remote);
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    sock_udp_close(&sock);
    return 0;
}
