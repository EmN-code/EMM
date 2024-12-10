
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void init_udp_socket(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Let the system assign a local port

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void publish_state(void) {
    const char *payload = (rand() % 2 == 0) ? "work" : "not work";
    if (sock_udp_send(&sock, payload, strlen(payload), &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void) {
    init_udp_socket();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
