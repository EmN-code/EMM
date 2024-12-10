
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "riot_esp32"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_udp_t mqtt_sock;
static sock_udp_ep_t broker_ep = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_client_t client;
    mqtt_client_init(&client, &mqtt_sock, &broker_ep, MQTT_CLIENT_ID, MQTT_KEEPALIVE);

    if (mqtt_client_connect(&client) == 0) {
        mqtt_client_publish(&client, &pub);
        mqtt_client_disconnect(&client);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    broker_ep.family = AF_INET;
    broker_ep.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    sock_udp_create(&mqtt_sock, NULL, &broker_ep, 0);

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}
