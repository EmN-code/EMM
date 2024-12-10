
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
#define MQTT_CLIENT_ID "esp32_client"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t _sock;
static mqtt_client_t _client;
static char _rx_buf[128];
static char _tx_buf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&_sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_connect_t conn = {
        .keep_alive = MQTT_KEEPALIVE,
        .client_id = MQTT_CLIENT_ID,
        .clean_session = 1,
        .will_flag = 0,
        .username = NULL,
        .password = NULL,
    };

    if (mqtt_connect(&_client, &conn, _tx_buf, sizeof(_tx_buf)) < 0) {
        puts("Error: failed to send MQTT connect packet");
        return;
    }

    if (mqtt_read_publish(&_client, _rx_buf, sizeof(_rx_buf)) < 0) {
        puts("Error: failed to receive MQTT connack packet");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_t pub = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    if (mqtt_publish(&_client, &pub, _tx_buf, sizeof(_tx_buf)) < 0) {
        puts("Error: failed to send MQTT publish packet");
        return;
    }
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    _mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        _mqtt_publish();
    }

    return 0;
}
