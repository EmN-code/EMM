
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t _sock;
static mqtt_client_t _client;
static char _send_buf[128];
static char _recv_buf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&_sock, &remote, &local, 0) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    mqtt_client_init(&_client, &_sock, _send_buf, sizeof(_send_buf), _recv_buf, sizeof(_recv_buf));

    mqtt_connect_t conn = {
        .keep_alive = 60,
        .client_id = MQTT_CLIENT_ID,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&_client, &conn) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void _mqtt_publish(void)
{
    mqtt_publish_t pub = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    if (mqtt_publish(&_client, &pub) < 0) {
        puts("Error: could not publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
