
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t _sock;
static mqtt_client_t _client;
static char _send_buf[128];
static char _recv_buf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&_sock, &remote, &local, 0);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    mqtt_connect_t conn = {
        .clean_session = true,
        .keep_alive = 60,
        .client_id = MQTT_CLIENT_ID,
    };

    res = mqtt_connect(&_client, &_sock, &conn, NULL, NULL);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_t pub = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS_0,
        .retain = false,
    };

    int res = mqtt_publish(&_client, &pub, NULL, NULL);
    if (res < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    puts("Starting MQTT client");

    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
