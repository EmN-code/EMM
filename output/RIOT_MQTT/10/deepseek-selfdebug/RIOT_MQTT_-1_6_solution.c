
#include <stdio.h>
#include <string.h>
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
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLIENT_ID "riot_esp32"

#define TIMER_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(broker_port);
    inet_pton(AF_INET, broker_ip, &broker_addr.sin_addr);

    mqtt_connect_params_t conn_params = {
        .keep_alive = MQTT_KEEPALIVE,
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .will_flag = 0,
    };

    mqtt_client_connect(client, (struct sockaddr *)&broker_addr, &conn_params);
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish_params_t pub_params = {
        .topic = topic,
        .message = message,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_client_publish(client, &pub_params);
}

int main(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
    }

    return 0;
}
