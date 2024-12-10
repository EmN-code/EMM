#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "net/af.h"
#include "string.h"
#include "xtimer.h"

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };

    // Convert the server address from string to endpoint.
    if (sock_udp_str2ep(&server, SERVER_ADDR) < 0) {
        printf("Error: unable to parse server address.\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: failed to create socket.\n");
        return -1;
    }

    const char *state = "work";
    while (1) {
        ssize_t res = sock_udp_send(&sock, state, strlen(state), &server);
        if (res < 0) {
            printf("Error: failed to send data.\n");
        } else {
            printf("Sent device state '%s' to server.\n", state);
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}