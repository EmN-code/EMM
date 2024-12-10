
#include <stdio.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work or not work"
#define MQTT_KEEPALIVE 360
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static sock_tcp_t _sock;
static mqtt_client_t _client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    int res = sock_tcp_connect(&_sock, &remote, &local, 0);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    mqtt_connect_msg_t conn_msg = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = true,
    };

    res = mqtt_connect(&_client, &_sock, &conn_msg);
    if (res < 0) {
        puts("Error sending MQTT connect message");
        return;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish_msg_t pub_msg = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = sizeof(MQTT_MESSAGE) - 1,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    int res = mqtt_publish(&_client, &pub_msg);
    if (res < 0) {
        puts("Error sending MQTT publish message");
    }
}

int main(void)
{
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
