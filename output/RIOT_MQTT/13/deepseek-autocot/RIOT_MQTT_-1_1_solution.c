
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/conn/udp.h>
#include <net/conn/tcp.h>
#include <net/conn/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
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

    mqtt_network_t network;
    mqtt_network_init(&network);
    mqtt_network_connect(&network, broker_ip, broker_port);
    mqtt_client_init(client, &network, 1024, NULL, NULL);
    mqtt_client_connect(client, &conn_info);
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_message_t msg;
    msg.topic = topic;
    msg.payload = (void *)message;
    msg.payload_len = strlen(message);
    msg.qos = 0;
    msg.retain = 0;

    mqtt_client_publish(client, &msg);
}

int main(void)
{
    mqtt_client_t client;

    puts("Connecting to MQTT broker...");
    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        puts("Publishing state...");
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
