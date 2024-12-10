
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;
static mqtt_connect_info_t conn_info;

void mqtt_publish(void) {
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = MQTT_PAYLOAD_LEN;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_publish(&client, &pub);
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    memset(&conn_info, 0, sizeof(conn_info));
    conn_info.client_id = MQTT_CLIENT_ID;
    conn_info.keep_alive = MQTT_KEEPALIVE;

    if (mqtt_connect(&client, &sock, &conn_info) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
