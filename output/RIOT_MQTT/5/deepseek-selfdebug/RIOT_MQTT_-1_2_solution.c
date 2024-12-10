
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

static void mqtt_publish(void)
{
    mqtt_publish_t pubmsg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_PAYLOAD,
        .payload_len = strlen(MQTT_PAYLOAD),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_publish(&mqtt_client, &pubmsg);
}

static void mqtt_connect(void)
{
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = MQTT_CLEAN_SESSION,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
    };

    mqtt_client_init(&mqtt_client);
    mqtt_client_connect(&mqtt_client, &mqtt_sock, &conn_info, MQTT_BROKER_IP, MQTT_BROKER_PORT);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&mqtt_sock, &local, &remote, 0);

    mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, MQTT_PUBLISH_INTERVAL);
        mqtt_publish();
    }

    return 0;
}
