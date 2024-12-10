
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = broker_port };
    inet_addr_from_str(broker_ip, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_client_init(client, &sock, NULL, 0, NULL, 0);
    mqtt_connect_data_t conn_data = MQTT_CONNECT_DATA_DEFAULT(MQTT_CLIENT_ID);
    mqtt_connect(client, &conn_data);
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message, size_t len) {
    mqtt_publish_data_t pub_data = {
        .topic = topic,
        .message = message,
        .msglen = len,
        .qos = MQTT_QOS_0,
        .retain = 0,
        .dup = 0
    };
    mqtt_publish(client, &pub_data);
}

int main(void) {
    mqtt_client_t client;
    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, MQTT_MESSAGE_LEN);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
