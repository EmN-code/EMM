
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
#define MQTT_KEEPALIVE 30
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_connect(sock_tcp_t *sock)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &addr.sin_addr);

    sock_tcp_connect(sock, (struct sockaddr *)&addr, 0);
}

static void mqtt_publish(sock_tcp_t *sock, const char *topic, const char *message)
{
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.message = message;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;
    pub.dup = 0;
    pub.id = 0;

    mqtt_publish(sock, &pub);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_create(&sock, NULL, NULL, SOCK_PROTO_TCP);

    mqtt_connect(&sock);

    mqtt_connect_t conn;
    conn.client_id = MQTT_CLIENT_ID;
    conn.keep_alive = MQTT_KEEPALIVE;
    conn.will_topic = MQTT_WILL_TOPIC;
    conn.will_message = MQTT_WILL_MESSAGE;
    conn.will_qos = MQTT_WILL_QOS;
    conn.will_retain = MQTT_WILL_RETAIN;
    conn.username = MQTT_USERNAME;
    conn.password = MQTT_PASSWORD;

    mqtt_connect(&sock, &conn);

    while (1) {
        mqtt_publish(&sock, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);

    return 0;
}
