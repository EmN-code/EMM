
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
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

void mqtt_publish(void) {
    mqtt_msg_t msg;
    mqtt_msg_publish_init(&msg);
    mqtt_msg_publish_topic(&msg, MQTT_TOPIC);
    mqtt_msg_publish_payload(&msg, MQTT_MESSAGE, strlen(MQTT_MESSAGE));
    mqtt_msg_publish_qos(&msg, MQTT_QOS);
    mqtt_msg_publish_retain(&msg, MQTT_RETAIN);

    mqtt_client_publish(&mqtt_client, &msg);
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    mqtt_client_init(&mqtt_client, &mqtt_sock);

    mqtt_connect_t conn;
    mqtt_connect_init(&conn);
    mqtt_connect_client_id(&conn, MQTT_CLIENT_ID);
    mqtt_connect_keep_alive(&conn, MQTT_KEEPALIVE);
    mqtt_connect_will_topic(&conn, MQTT_WILL_TOPIC);
    mqtt_connect_will_message(&conn, MQTT_WILL_MESSAGE);
    mqtt_connect_will_qos(&conn, MQTT_WILL_QOS);
    mqtt_connect_will_retain(&conn, MQTT_WILL_RETAIN);
    mqtt_connect_username(&conn, MQTT_USERNAME);
    mqtt_connect_password(&conn, MQTT_PASSWORD);

    if (mqtt_client_connect(&mqtt_client, &conn) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        xtimer_usleep(TIMER_INTERVAL);
        mqtt_publish();
    }

    return 0;
}
