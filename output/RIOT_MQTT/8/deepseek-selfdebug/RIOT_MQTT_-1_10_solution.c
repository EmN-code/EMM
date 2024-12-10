
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
#define MQTT_KEEPALIVE 30
#define MQTT_CLIENT_ID "esp32_client"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&sock, &remote, &local, 0);
    if (res < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = true,
    };

    res = mqtt_connect(&client, &sock, &conn_info, NULL, NULL);
    if (res < 0) {
        puts("Error: failed to connect to MQTT broker");
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
        .retain = MQTT_RETAIN,
    };

    int res = mqtt_publish(&client, &pub_info);
    if (res < 0) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published successfully");
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
