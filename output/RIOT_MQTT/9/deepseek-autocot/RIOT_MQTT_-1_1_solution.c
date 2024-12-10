
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t _sock;
static mqtt_client_t _client;
static char _sendbuf[128];
static char _readbuf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, BROKER_IP);

    sock_tcp_create(&_sock, &local, &remote, 0);
    mqtt_client_init(&_client, &_sock, _sendbuf, sizeof(_sendbuf), _readbuf, sizeof(_readbuf), NULL);

    mqtt_connect_info_t conn_info = {
        .client_id = CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    mqtt_connect(&_client, &conn_info);
}

static void _mqtt_publish(void)
{
    mqtt_publish_info_t pub_info = {
        .topic = TOPIC,
        .payload = MESSAGE,
        .payload_len = strlen(MESSAGE),
        .qos = 0,
        .retain = 0
    };

    mqtt_publish(&_client, &pub_info);
}

int main(void)
{
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
