
#include <stdio.h>
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

static void mqtt_publish(sock_tcp_t *sock, const char *topic, const char *message)
{
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.message = message;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;
    pub.dup = 0;
    pub.message_id = 0;

    mqtt_publish_simple(sock, &pub);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_t broker_addr;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr);
    remote.addr.ipv4 = broker_addr;

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    mqtt_connect_t conn;
    conn.client_id = MQTT_CLIENT_ID;
    conn.keep_alive = MQTT_KEEPALIVE;
    conn.username = MQTT_USERNAME;
    conn.password = MQTT_PASSWORD;

    if (mqtt_connect(&sock, &conn) < 0) {
        puts("Error connecting to MQTT broker");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        mqtt_publish(&sock, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}
