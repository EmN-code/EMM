
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "esp32_client"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    sock_udp_ep_t broker = { .family = AF_INET, .port = broker_port };
    inet_pton(AF_INET, broker_ip, &broker.addr.ipv4);

    mqtt_connect_info_t connect_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = MQTT_CLEAN_SESSION,
    };

    mqtt_client_connect(client, &broker, &connect_info, NULL, NULL);
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *payload)
{
    mqtt_publish_info_t publish_info = {
        .topic = topic,
        .payload = payload,
        .payload_len = strlen(payload),
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

    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        if (xtimer_expired(&timer)) {
            mqtt_publish(&client, MQTT_TOPIC, MQTT_PAYLOAD);
            xtimer_set(&timer, TIMER_INTERVAL);
        }
        xtimer_spin(&timer);
    }

    return 0;
}
