
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/netopt.h>
#include <net/af.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 10000

static sock_tcp_t _sock;
static mqtt_client_t _client;
static char _sendbuf[128];
static char _readbuf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    if (sock_tcp_connect(&_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_client_init(&_client, &_sock, _sendbuf, sizeof(_sendbuf), _readbuf, sizeof(_readbuf), NULL);

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
    };

    if (mqtt_connect(&_client, &conn_info) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
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

    if (mqtt_publish(&_client, &pub_info) < 0) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    if (netif_set_opt(netif, NETOPT_STATE, 0, &(netopt_state_t){ NETOPT_STATE_ON }, sizeof(netopt_state_t)) < 0) {
        puts("Error: failed to bring up network interface");
        return 1;
    }

    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
