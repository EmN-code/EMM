
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, &local, 0) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    mqtt_client_init(&mqtt_client);
    mqtt_connect_msg_t connect_msg = {
        .keep_alive = MQTT_KEEPALIVE,
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .clean_session = 1,
    };

    if (mqtt_connect(&mqtt_client, &connect_msg, &mqtt_sock) < 0) {
        puts("Error: could not send MQTT connect message");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_msg_t publish_msg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    if (mqtt_publish(&mqtt_client, &publish_msg, &mqtt_sock) < 0) {
        puts("Error: could not send MQTT publish message");
    }
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    _mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        _mqtt_publish();
    }

    return 0;
}
