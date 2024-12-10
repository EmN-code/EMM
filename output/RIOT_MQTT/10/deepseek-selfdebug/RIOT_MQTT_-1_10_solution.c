
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
#define MQTT_KEEPALIVE 30
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static void mqtt_connect(sock_tcp_t *sock) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &server.sin_addr);

    sock_tcp_connect(sock, (struct sockaddr *)&server, sizeof(server));
}

static void mqtt_publish(sock_tcp_t *sock, const char *topic, const char *message) {
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.message = message;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_publish(sock, &pub);
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_create(&sock, NULL, NULL, 0);

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}
