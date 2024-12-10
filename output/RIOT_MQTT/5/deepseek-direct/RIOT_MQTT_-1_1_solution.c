
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
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "pass"

#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(int sock) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .payload = states[state],
        .payload_len = strlen(states[state]),
        .qos = MQTT_QOS_0,
        .retain = false,
    };

    mqtt_publish(sock, &pub_params);

    state = !state; // Toggle state
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to MQTT broker");
        return 1;
    }

    mqtt_connect_params_t conn_params = {
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .keep_alive = 60,
        .clean_session = true,
    };

    if (mqtt_connect(sock, &conn_params) < 0) {
        puts("Error: could not connect to MQTT broker");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state(sock);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
