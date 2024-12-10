
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/ipv6/addr.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netreg.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/udp.h>
#include <net/gnrc/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN (sizeof(MQTT_MESSAGE) - 1)
#define MQTT_PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t mqtt_client;

static void _mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    ipv4_addr_t broker_addr;

    if (ipv4_addr_from_str(&broker_addr, MQTT_BROKER_IP) == NULL) {
        puts("Error: invalid broker IP address");
        return;
    }

    remote.addr.ipv4 = broker_addr;

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    mqtt_client_init(&mqtt_client, &sock);

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = true,
    };

    if (mqtt_connect(&mqtt_client, &conn_info) != 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void _mqtt_publish(void) {
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = MQTT_MESSAGE_LEN,
        .qos = MQTT_QOS,
        .retain = false,
    };

    if (mqtt_publish(&mqtt_client, &pub_info) != 0) {
        puts("Error: could not publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    puts("Starting MQTT client");

    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}
