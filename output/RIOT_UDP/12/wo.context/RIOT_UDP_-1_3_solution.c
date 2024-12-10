#include <string.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <xtimer.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

static const char *work_state = "work";
static const char *not_work_state = "not work";

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    int res = inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: unable to parse IP address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        const char *state = work_state;  // This should be dynamically decided.
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            puts("Error: unable to send UDP packet\n");
        } else {
            printf("Sent state: %s\n", state);
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}