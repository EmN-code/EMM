
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "mqtt.h"
#include "net/sock/tcp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4

static struct mqtt_connection conn;
static uint16_t mid;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_tcp_t sock;
    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    mqtt_register(&conn, NULL, "nrf52840_client", mqtt_event_callback, 0);
    mqtt_set_username_password(&conn, NULL, NULL);

    while (1) {
        mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
        xtimer_sleep(5);
    }

    return 0;
}
