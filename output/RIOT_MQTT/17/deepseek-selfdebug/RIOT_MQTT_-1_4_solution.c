
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define INTERVAL_MS (5 * 1000000U)

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void init_udp_socket(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    remote.port = MQTT_BROKER_PORT;
    remote.family = AF_INET;

    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error converting IP address");
        return;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void publish_state(void) {
    uint8_t buf[128];
    size_t len = snprintf((char *)buf, sizeof(buf), "PUBLISH %s %s", MQTT_TOPIC, MQTT_PAYLOAD);

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Published: %s\n", MQTT_PAYLOAD);
    }
}

int main(void) {
    init_udp_socket();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
