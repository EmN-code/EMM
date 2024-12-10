
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

static void mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_client_init(&mqtt_client);
    mqtt_connect_client(&mqtt_client, &mqtt_sock, MQTT_CLIENT_ID, NULL, NULL, MQTT_KEEPALIVE);
}

static void mqtt_publish(void) {
    mqtt_publish_client(&mqtt_client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        mqtt_publish();
    }

    return 0;
}
