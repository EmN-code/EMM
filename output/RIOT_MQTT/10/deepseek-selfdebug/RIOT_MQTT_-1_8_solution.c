
#include <stdio.h>
#include <thread.h>
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

static void mqtt_publish(void) {
    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    sock_tcp_ep_t broker = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };

    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, NULL, &broker, 0);

    mqtt_client_init(&client, &sock, NULL, NULL, NULL, NULL);

    mqtt_connect(&client, &conn_info);

    mqtt_publish_simple(&client, MQTT_TOPIC, MQTT_QOS, MQTT_RETAIN, MQTT_MESSAGE, sizeof(MQTT_MESSAGE) - 1);

    mqtt_disconnect(&client);
    sock_tcp_close(&sock);
}

int main(void) {
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
    return 0;
}
