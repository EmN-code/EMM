
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "esp32_client"

#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t _sock;
static mqtt_connection_t _conn;

static int _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    if (sock_tcp_connect(&_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return -1;
    }

    mqtt_connect_init(&_conn, (uint8_t *)MQTT_CLIENT_ID, 0, NULL, NULL, 0, 0);
    if (mqtt_connect(&_conn, &_sock) != MQTT_OK) {
        puts("Error: MQTT connect failed");
        return -1;
    }

    return 0;
}

static void _mqtt_publish(void)
{
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = strlen(MQTT_PAYLOAD);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    if (mqtt_publish(&_conn, &pub) != MQTT_OK) {
        puts("Error: MQTT publish failed");
    } else {
        puts("State published");
    }
}

int main(void)
{
    if (_mqtt_connect() < 0) {
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        _mqtt_publish();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
