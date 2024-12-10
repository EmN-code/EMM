
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void _mqtt_connect(mqtt_client_t *client, const sock_udp_ep_t *broker)
{
    mqtt_connect_info_t conn_info = {
        .client_id = CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };

    int res = mqtt_connect(client, broker, &conn_info, NULL, NULL);
    if (res != MQTT_OK) {
        printf("MQTT connect failed: %d\n", res);
        return;
    }
    printf("MQTT connected\n");
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish_info_t pub_info = {
        .topic = topic,
        .payload = message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    int res = mqtt_publish(client, &pub_info);
    if (res != MQTT_OK) {
        printf("MQTT publish failed: %d\n", res);
        return;
    }
    printf("MQTT message published: %s\n", message);
}

int main(void)
{
    sock_udp_ep_t broker = {
        .family = AF_INET,
        .port = BROKER_PORT,
    };

    if (sock_udp_str2ep(&broker, BROKER_IP) < 0) {
        printf("Invalid broker IP address\n");
        return 1;
    }

    mqtt_client_t client;
    mqtt_client_init(&client);

    _mqtt_connect(&client, &broker);

    while (1) {
        _mqtt_publish(&client, TOPIC, MESSAGE);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
