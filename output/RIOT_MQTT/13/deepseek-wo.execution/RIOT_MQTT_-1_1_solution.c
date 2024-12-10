
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/conn/udp.h>
#include <net/conn/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 10000

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port) {
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(broker_port);
    inet_pton(AF_INET, broker_ip, &broker_addr.sin_addr);

    mqtt_connect_server(client, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message) {
    mqtt_publish_simple(client, topic, message, strlen(message), MQTT_QOS, MQTT_RETAIN);
}

int main(void) {
    mqtt_client_t client;
    mqtt_client_init(&client);

    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
