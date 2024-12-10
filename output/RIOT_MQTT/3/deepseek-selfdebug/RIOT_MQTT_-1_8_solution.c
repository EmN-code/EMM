
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
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
#define MQTT_WILL_FLAG 0
#define MQTT_CONNECT_TIMEOUT_MS 10000
#define MQTT_PUBLISH_INTERVAL_MS 5000

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

static void mqtt_connect(void)
{
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_client_config_t config = {
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = MQTT_CLEAN_SESSION,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
        .will_flag = MQTT_WILL_FLAG
    };

    if (mqtt_client_connect(&mqtt_client, &mqtt_sock, &config) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(void)
{
    mqtt_publish_t pub = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_PAYLOAD,
        .payload_len = strlen(MQTT_PAYLOAD),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    if (mqtt_client_publish(&mqtt_client, &pub) < 0) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    sock_tcp_init(&mqtt_sock);
    mqtt_client_init(&mqtt_client);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(MQTT_PUBLISH_INTERVAL_MS / MS_PER_SEC);
    }

    return 0;
}
