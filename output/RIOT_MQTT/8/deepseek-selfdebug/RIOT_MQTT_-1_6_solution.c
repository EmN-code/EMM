
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

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&mqtt_sock, &remote, &local, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    mqtt_client_init(&mqtt_client);
    mqtt_connect_params_t connect_params = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL,
    };

    res = mqtt_connect(&mqtt_client, &mqtt_sock, &connect_params);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_params_t publish_params = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    int res = mqtt_publish(&mqtt_client, &publish_params);
    if (res < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    _mqtt_connect();

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        _mqtt_publish();
    }

    return 0;
}
