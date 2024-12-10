
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;

void mqtt_publish(void) {
    mqtt_publish_t pubmsg;
    pubmsg.topic = MQTT_TOPIC;
    pubmsg.payload = MQTT_MESSAGE;
    pubmsg.payload_len = strlen(MQTT_MESSAGE);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retain = MQTT_RETAIN;

    mqtt_publish(&client, &pubmsg);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD
    };

    if (mqtt_connect(&client, &sock, &conn_info) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    xtimer_t timer;
    timer.callback = mqtt_publish;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
