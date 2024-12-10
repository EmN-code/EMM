
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void mqtt_connect(void) {
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&sock, NULL, &remote, 0);
}

void mqtt_publish(void) {
    uint8_t buf[128];
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = MQTT_PAYLOAD_LEN;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_publish_simple(&sock, &pub, buf, sizeof(buf));
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}
