
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"

static void mqtt_connect(sock_tcp_t *sock) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(sock_tcp_t *sock) {
    mqtt_publish_t pubmsg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = sizeof(MQTT_MESSAGE) - 1,
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    if (mqtt_publish(sock, &pubmsg) < 0) {
        puts("Error: failed to publish message");
        return;
    }

    puts("Message published");
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: failed to create socket");
        return 1;
    }

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock);
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}