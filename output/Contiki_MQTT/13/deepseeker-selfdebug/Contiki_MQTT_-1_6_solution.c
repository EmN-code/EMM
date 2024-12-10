
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <mqtt.h>
#include <net/sock/tcp.h>

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS MQTT_QOS_0

static struct mqtt_connection conn;

void publish_state(void) {
    char message[20];
    static int state = 0;

    if (state == 0) {
        strcpy(message, "work");
        state = 1;
    } else {
        strcpy(message, "not work");
        state = 0;
    }

    mqtt_publish(&conn, MQTT_TOPIC, message, strlen(message), MQTT_QOS, false);
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_tcp_ep_t remote;
    sock_tcp_t sock;

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_HOST, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    mqtt_init(&conn, &sock);

    if (mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 1, NULL) != MQTT_OK) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    mqtt_disconnect(&conn);
    return 0;
}
