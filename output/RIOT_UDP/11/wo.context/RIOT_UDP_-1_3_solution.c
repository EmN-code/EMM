#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <kernel_defines.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)  // Send every 5 seconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
static const char *state_msg = "work";

int main(void) {
    /* Convert the IP address */
    if (inet_pton(AF_INET, SERVER_IP, &server.addr) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    /* Create socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        sock_udp_send(&sock, state_msg, strlen(state_msg), &server);
        printf("Sent state: %s\n", state_msg);
        xtimer_sleep(5);
    }

    return 0;
}