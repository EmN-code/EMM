
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
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;
static mqtt_connect_info_t conn_info;

void mqtt_publish(void) {
    mqtt_publish_t pub_msg;
    pub_msg.topic = MQTT_TOPIC;
    pub_msg.payload = MQTT_MESSAGE;
    pub_msg.payload_len = strlen(MQTT_MESSAGE);
    pub_msg.qos = MQTT_QOS;
    pub_msg.retain = MQTT_RETAIN;

    mqtt_publish(&client, &pub_msg);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize TCP socket
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    // Initialize MQTT client
    mqtt_init(&client, &sock);

    // Set up connection info
    conn_info.client_id = MQTT_CLIENT_ID;
    conn_info.keep_alive = MQTT_KEEPALIVE;
    conn_info.clean_session = 1;
    conn_info.username = MQTT_USERNAME;
    conn_info.password = MQTT_PASSWORD;

    // Connect to MQTT broker
    if (mqtt_connect(&client, &conn_info) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        mqtt_publish();
    }

    return 0;
}
