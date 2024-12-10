
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(sock_tcp_t *sock) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(sock_tcp_t *sock) {
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    if (mqtt_publish(sock, &pub) < 0) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
