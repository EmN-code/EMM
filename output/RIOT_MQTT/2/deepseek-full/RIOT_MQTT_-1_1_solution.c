
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static network_t network;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

void publish_state(mqtt_client_t *client, const char *payload) {
    mqtt_publish_params_t pub_params = {
        .topic = TOPIC,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = MQTT_QOS_0,
        .retain = 0,
        .dup = 0
    };

    mqtt_publish(client, &pub_params);
}

void connect_to_broker(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&sock, &local, &remote, 0);

    network_init(&network, (sock_udp_t *)&sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_client_init(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect_params_t conn_params = {
        .keep_alive = 60,
        .client_id = "riot_esp32",
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .username = NULL,
        .password = NULL
    };

    mqtt_connect(&client, &conn_params);
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state(&client, PAYLOAD_WORK);
        xtimer_usleep(INTERVAL);
        publish_state(&client, PAYLOAD_NOT_WORK);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
