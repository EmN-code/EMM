
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };

void publish_state(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "PUBLISH %s %s\n", TOPIC, PAYLOAD);
    if (sock_udp_send(&sock, buf, len, &broker) < 0) {
        puts("Error sending message");
    } else {
        puts("State published");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the local endpoint
    local.port = 12345;  // Arbitrary local port
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set the broker endpoint
    inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4);

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
