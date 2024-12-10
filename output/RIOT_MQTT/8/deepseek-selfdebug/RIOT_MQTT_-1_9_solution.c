
#include <stdio.h>
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
#define MQTT_CLEAN_SESSION 1
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static void mqtt_publish(void)
{
    static int state = 0;
    const char *message = (state++ % 2 == 0) ? "work" : "not work";

    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = MQTT_CLEAN_SESSION,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
    };

    sock_tcp_ep_t broker = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT,
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, NULL, &broker, 0);
    mqtt_client_init(&client, &sock);

    mqtt_connect(&client, &conn_info);
    mqtt_publish(&client, MQTT_TOPIC, message, strlen(message), MQTT_QOS, MQTT_RETAIN);
    mqtt_disconnect(&client);
    sock_tcp_close(&sock);
}

int main(void)
{
    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}
