
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_DELAY 5000000UL // 5 seconds in microseconds

static void mqtt_connect(int sock) {
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
    };

    mqtt_connect(sock, &conn_info);
}

static void mqtt_publish(int sock) {
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_publish(sock, &pub_info);
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote;

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    mqtt_connect(sock.fd);

    while (1) {
        mqtt_publish(sock.fd);
        xtimer_usleep(MQTT_DELAY);
    }

    sock_tcp_close(&sock);
    return 0;
}
