
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_connect(sock_tcp_t *sock) {
    struct mqtt_connect_client_info_t ci;
    mqtt_client_t *client;

    memset(&ci, 0, sizeof(ci));
    ci.client_id = "riot_esp32";
    ci.keep_alive = MQTT_KEEPALIVE;
    ci.username = MQTT_USERNAME;
    ci.password = MQTT_PASSWORD;

    client = mqtt_client_new();
    if (client == NULL) {
        puts("Failed to create MQTT client");
        return;
    }

    if (mqtt_client_connect(client, sock, MQTT_BROKER_IP, MQTT_BROKER_PORT, mqtt_connection_cb, &ci) != 0) {
        puts("MQTT connection failed");
        mqtt_client_disconnect(client);
        mqtt_client_delete(client);
        return;
    }

    puts("MQTT connected");
}

static void mqtt_publish(mqtt_client_t *client) {
    mqtt_publish_t pub;
    memset(&pub, 0, sizeof(pub));
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    if (mqtt_publish(client, &pub) != 0) {
        puts("MQTT publish failed");
    } else {
        puts("MQTT message published");
    }
}

int main(void) {
    sock_tcp_t sock;
    mqtt_client_t *client;

    if (sock_tcp_create(&sock, NULL, NULL, SOCK_ADDR_ANY_NETIF, SOCK_ADDR_ANY_PORT, SOCK_PROTO_TCP) < 0) {
        puts("Failed to create TCP socket");
        return 1;
    }

    mqtt_connect(&sock);

    client = mqtt_client_new();
    if (client == NULL) {
        puts("Failed to create MQTT client");
        return 1;
    }

    while (1) {
        mqtt_publish(client);
        xtimer_sleep(5);
    }

    mqtt_client_disconnect(client);
    mqtt_client_delete(client);
    sock_tcp_close(&sock);

    return 0;
}
