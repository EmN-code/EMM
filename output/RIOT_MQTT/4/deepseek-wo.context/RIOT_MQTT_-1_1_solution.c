
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 10000

static void _mqtt_connect(mqtt_client_t *client, const char *client_id, const char *username, const char *password)
{
    mqtt_connect_info_t conn_info = {
        .client_id = client_id,
        .username = username,
        .password = password,
        .keep_alive = MQTT_KEEPALIVE,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .clean_session = 1,
    };

    mqtt_client_connect(client, &conn_info);
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish_info_t pub_info = {
        .topic = topic,
        .payload = message,
        .payload_len = strlen(message),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_client_publish(client, &pub_info);
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_client_t client;
    mqtt_client_init(&client, &sock, NULL, 0, NULL, 0);

    _mqtt_connect(&client, MQTT_CLIENT_ID, NULL, NULL);

    while (1) {
        _mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
