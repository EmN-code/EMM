
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
#define MQTT_CLIENT_ID "riot_esp32"

static sock_tcp_t sock;
static mqtt_client_t client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating TCP socket");
        return;
    }

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

    res = mqtt_client_connect(&client, &sock, &conn_info);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    int res = mqtt_client_publish(&client, &pub_info);
    if (res < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
