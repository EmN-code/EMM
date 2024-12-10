
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 360
#define MQTT_CLIENT_ID "esp32_client"

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

void mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_client_init(&mqtt_client, &mqtt_sock);
    mqtt_connect_params_t conn_params = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = true,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&mqtt_client, &conn_params) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void mqtt_publish(void) {
    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN
    };

    if (mqtt_publish(&mqtt_client, &pub_params) < 0) {
        puts("Error: failed to publish message");
        return;
    }

    puts("Message published");
}

int main(void) {
    puts("Starting MQTT client");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
