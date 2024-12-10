
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    mqtt_client_init(&client);
    mqtt_network_t network;
    network.socket = &sock;
    network.connect = (mqtt_network_connect_t)sock_udp_create;
    network.disconnect = (mqtt_network_disconnect_t)sock_udp_disconnect;
    network.send = (mqtt_network_send_t)sock_udp_send;
    network.recv = (mqtt_network_recv_t)sock_udp_recv;

    mqtt_connect_info_t conn_info = {
        .client_id = "riot_esp32",
        .username = NULL,
        .password = NULL,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0
    };

    mqtt_client_connect(&client, &network, &conn_info, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

void mqtt_publish(void) {
    mqtt_publish_info_t pub_info = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = strlen(PAYLOAD),
        .qos = 0,
        .retain = 0
    };

    mqtt_client_publish(&client, &pub_info, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
