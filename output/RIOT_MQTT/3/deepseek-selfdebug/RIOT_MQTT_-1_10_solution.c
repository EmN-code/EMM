
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;

void mqtt_publish(void) {
    mqtt_publish_t pubmsg;
    pubmsg.topic = MQTT_TOPIC;
    pubmsg.payload = MQTT_MESSAGE;
    pubmsg.payload_len = strlen(MQTT_MESSAGE);
    pubmsg.qos = MQTT_QOS_0;
    pubmsg.retain = 0;

    mqtt_client_publish(&mqtt_client, &pubmsg);
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&mqtt_sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    mqtt_client_t client;
    mqtt_client_init(&client, &mqtt_sock);

    mqtt_connect_t conn;
    conn.client_id = MQTT_CLIENT_ID;
    conn.keep_alive = 60;
    conn.clean_session = 1;
    conn.will_flag = 0;

    if (mqtt_client_connect(&client, &conn) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
