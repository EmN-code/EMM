
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_udp_ep_t broker;

static void connect_to_broker(void) {
    int result;
    result = inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return;
    }
    broker.port = MQTT_BROKER_PORT;
    broker.family = AF_INET;

    if (sock_tcp_connect(&sock, &broker, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

static void publish_state(void) {
    mqtt_publish_params_t pub_params;
    pub_params.qos = MQTT_QOS_0;
    pub_params.retain = false;
    pub_params.topic = MQTT_TOPIC;
    pub_params.message = MQTT_MESSAGE;
    pub_params.message_len = strlen(MQTT_MESSAGE);

    if (mqtt_publish(&sock, &pub_params) < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    connect_to_broker();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, MQTT_INTERVAL);
        publish_state();
    }

    return 0;
}
