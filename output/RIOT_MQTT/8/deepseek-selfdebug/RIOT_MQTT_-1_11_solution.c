
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
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

static void mqtt_connect(void) {
    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    mqtt_client_init(&client);
    mqtt_client_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &conn_info);
}

static void mqtt_publish(void) {
    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_msg_t msg;
    mqtt_msg_init(&client, &msg, MQTT_TOPIC, strlen(MQTT_TOPIC));
    mqtt_msg_payload(&msg, MQTT_MESSAGE, strlen(MQTT_MESSAGE));
    mqtt_msg_qos(&msg, MQTT_QOS);
    mqtt_msg_retain(&msg, MQTT_RETAIN);

    mqtt_client_publish(&client, &msg);
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
