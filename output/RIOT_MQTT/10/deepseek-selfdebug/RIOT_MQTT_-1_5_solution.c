
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    char buf[64];
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = 0;
    pub.retain = 0;

    mqtt_publish(&sock, &broker, &pub, buf, sizeof(buf));
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    broker.port = MQTT_BROKER_PORT;
    broker.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    if (sock_udp_create(&sock, &local, &broker, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
