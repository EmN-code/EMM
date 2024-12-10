
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void init_udp_socket(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

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
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Published: %s\n", buf);
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
