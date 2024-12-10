
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
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define INTERVAL_MS (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t broker;

static void connect_to_broker(void) {
    broker.family = AF_INET;
    broker.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    int res = sock_tcp_connect(&sock, &broker, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

static void publish_state(void) {
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.message = MQTT_MESSAGE;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;
    pub.message_len = strlen(MQTT_MESSAGE);

    int res = mqtt_publish(&sock, &pub);
    if (res < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    sock_tcp_init(&sock);
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
