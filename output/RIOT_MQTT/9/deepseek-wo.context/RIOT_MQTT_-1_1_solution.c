
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

static void _mqtt_connect(void)
{
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL,
    };

    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &conn_info);
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

    mqtt_publish(&client, &pub_info);
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_udp_create(&sock, &local, NULL, 0);

    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
