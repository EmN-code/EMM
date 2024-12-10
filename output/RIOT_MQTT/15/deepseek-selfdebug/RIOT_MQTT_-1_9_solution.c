
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static sock_udp_t mqtt_sock;
static sock_udp_ep_t mqtt_broker = SOCK_IPV4_EP_ANY;

void mqtt_connect(void) {
    mqtt_broker.port = MQTT_BROKER_PORT;
    mqtt_broker.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &mqtt_broker.addr.ipv4);

    if (sock_udp_create(&mqtt_sock, NULL, &mqtt_broker, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    mqtt_connect_client(&mqtt_sock, MQTT_CLIENT_ID, MQTT_KEEPALIVE, MQTT_USERNAME, MQTT_PASSWORD);
}

void mqtt_publish(const char *topic, const char *message) {
    mqtt_publish_simple(&mqtt_sock, topic, message, MQTT_QOS, MQTT_RETAIN);
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
