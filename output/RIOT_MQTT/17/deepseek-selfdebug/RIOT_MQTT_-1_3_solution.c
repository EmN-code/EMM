
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_PAYLOADLEN sizeof(MQTT_MESSAGE)
#define MQTT_TIMEOUT 5000000

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(void) {
    char buf[128];
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payloadlen = MQTT_PAYLOADLEN;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;
    pub.dup = MQTT_DUP;
    pub.id = MQTT_ID;

    mqtt_publish(&sock, &pub, buf, sizeof(buf));
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(MQTT_TIMEOUT);
    }

    sock_udp_close(&sock);
    return 0;
}
