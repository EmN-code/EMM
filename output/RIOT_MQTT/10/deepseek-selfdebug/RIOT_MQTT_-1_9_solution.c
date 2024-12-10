
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static sock_tcp_t mqtt_sock;

static int mqtt_connect(void) {
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    return 0;
}

static int mqtt_publish(const char *topic, const char *message) {
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.message = message;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    if (mqtt_publish(&mqtt_sock, &pub) < 0) {
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

        xtimer_sleep(5);
    }

    sock_tcp_close(&mqtt_sock);
    return 0;
}
