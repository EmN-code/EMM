
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;

void init_network(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    broker.port = BROKER_PORT;
    broker.family = AF_INET;
    inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4);

    if (sock_udp_create(&sock, &local, &broker, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void publish_state(void) {
    mqtt_publish_t pub;
    pub.topic = TOPIC;
    pub.payload = PAYLOAD;
    pub.payload_len = strlen(PAYLOAD);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    if (mqtt_publish(&sock, &pub) < 0) {
        puts("Error publishing message");
    } else {
        puts("State published");
    }
}

int main(void) {
    init_network();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
