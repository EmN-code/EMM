
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static void mqtt_connect(int sock) {
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };
    mqtt_connect(sock, &conn_info);
}

static void mqtt_publish(int sock) {
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };
    mqtt_publish(sock, &pub_info);
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote = { .family = AF_INET };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }
    remote.port = MQTT_BROKER_PORT;

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to MQTT broker");
        return 1;
    }

    mqtt_connect(sock.fd);

    while (1) {
        mqtt_publish(sock.fd);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
