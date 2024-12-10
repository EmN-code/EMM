
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define INTERVAL_MS (5 * 1000U)

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    sock_tcp_ep_t broker_ep = { .family = AF_INET, .port = broker_port };
    inet_pton(AF_INET, broker_ip, &broker_ep.addr.ipv4);

    mqtt_connect_info_t connect_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
    };

    mqtt_client_connect(client, &broker_ep, &connect_info, NULL, NULL);
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish_info_t publish_info = {
        .topic = topic,
        .payload = message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_client_publish(client, &publish_info, NULL, NULL);
}

int main(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(INTERVAL_MS / MS_PER_SEC);
    }

    return 0;
}
