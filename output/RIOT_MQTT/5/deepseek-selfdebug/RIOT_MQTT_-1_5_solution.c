
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/tcp.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static char buf[128];

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    int len = snprintf(buf, sizeof(buf), "PUBLISH /state %s\n", state);

    if (sock_tcp_send(&sock, buf, len) < 0) {
        puts("Failed to send data");
    } else {
        puts("State published successfully");
    }
}

int main(void) {
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    if (sock_tcp_create(&sock, &remote, NULL, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Failed to connect to server");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    sock_tcp_disconnect(&sock);
    return 0;
}
