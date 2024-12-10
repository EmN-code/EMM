
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;

static int mqtt_connect(void) {
    sock_udp_ep_t local = SOCK_ADDR_ANY_NETIF;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    return 0;
}

static int mqtt_publish(const char *topic, const char *message) {
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.payload = message;
    pub.payload_len = strlen(message);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    if (mqtt_publish(&sock, &pub) < 0) {
        printf("Failed to publish message\n");
        return -1;
    }

    return 0;
}

int main(void) {
    if (mqtt_connect() < 0) {
        return -1;
    }

    while (1) {
        if (mqtt_publish(MQTT_TOPIC, MQTT_MESSAGE) < 0) {
            break;
        }
        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
