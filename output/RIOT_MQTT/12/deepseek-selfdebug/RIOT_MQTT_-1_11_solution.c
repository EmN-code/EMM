
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

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    sock_tcp_ep_t broker_addr = { .family = AF_INET, .port = broker_port };
    inet_pton(AF_INET, broker_ip, &broker_addr.addr.ipv4);

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };

    mqtt_client_connect(client, &broker_addr, &conn_info, NULL, NULL);
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish_info_t pub_info = {
        .topic = topic,
        .payload = message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_client_publish(client, &pub_info, NULL, NULL);
}

int main(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
