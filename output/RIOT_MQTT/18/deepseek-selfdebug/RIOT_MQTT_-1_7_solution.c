
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;

void publish_state(void) {
    mqtt_publish_params_t pub_params = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = strlen(PAYLOAD),
        .qos = MQTT_QOS_0,
        .retain = false,
    };

    mqtt_publish(&client, &pub_params);
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .port = SERVER_PORT, .family = AF_INET };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        puts("Error connecting to server");
        return 1;
    }

    mqtt_client_config_t config = {
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = true,
    };

    if (mqtt_client_init(&client, &sock, &config) < 0) {
        puts("Error initializing MQTT client");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
