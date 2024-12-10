
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
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0

static sock_udp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    mqtt_client_init(&client);
    mqtt_client_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &conn_info);
}

void mqtt_publish(const char *topic, const char *payload) {
    mqtt_publish_info_t pub_info = {
        .topic = topic,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = MQTT_QOS,
        .retain = 0
    };

    mqtt_client_publish(&client, &pub_info);
}

int main(void) {
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_udp_create(&sock, &local, NULL, 0);

    mqtt_connect();

    while (1) {
        const char *state = (random_real_inclusive() > 0.5) ? "work" : "not work";
        mqtt_publish(MQTT_TOPIC, state);
        xtimer_sleep(5);
    }

    return 0;
}
