
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static char state[] = "work"; // or "not work"

void publish_state(void) {
    char message[64];
    sprintf(message, "state: %s", state);

    sock_tcp_send(sock, message, strlen(message));
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, SOCK_TCP_MODE_DEFAULT) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    sock_tcp_close(sock);
    return 0;
}
