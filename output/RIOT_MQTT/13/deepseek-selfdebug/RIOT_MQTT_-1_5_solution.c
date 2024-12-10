
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

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
    mqtt_connect_params_t connect_params = {
        .client_id = MQTT_CLIENT_ID,
        .clean_session = 1,
        .keep_alive = 60
    };

    if (mqtt_connect(&mqtt_client, &connect_params) != MQTT_OK) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void mqtt_publish(void) {
    mqtt_publish_params_t publish_params = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = sizeof(MQTT_MESSAGE) - 1,
        .qos = MQTT_QOS_0,
        .retain = 0
    };

    if (mqtt_publish(&mqtt_client, &publish_params) != MQTT_OK) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
        mqtt_publish();
    }

    return 0;
}
