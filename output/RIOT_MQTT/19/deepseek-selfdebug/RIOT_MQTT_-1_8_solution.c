
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void init_udp_socket(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0; // Let the system assign a local port

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void publish_state(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "PUBLISH %s %s", MQTT_TOPIC, MQTT_MESSAGE);
    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("State published");
    }
}

int main(void) {
    init_udp_socket();

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}