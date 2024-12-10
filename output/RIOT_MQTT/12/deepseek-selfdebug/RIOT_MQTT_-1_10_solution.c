
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
#define MQTT_KEEPALIVE 60
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(sock_tcp_t *sock)
{
    struct mqtt_connect_client_info_t ci;
    memset(&ci, 0, sizeof(ci));
    ci.client_id = MQTT_CLIENT_ID;
    ci.keep_alive = MQTT_KEEPALIVE;
    ci.will_topic = MQTT_WILL_TOPIC;
    ci.will_message = MQTT_WILL_MESSAGE;
    ci.will_qos = MQTT_WILL_QOS;
    ci.will_retain = MQTT_WILL_RETAIN;
    ci.username = MQTT_USERNAME;
    ci.password = MQTT_PASSWORD;

    mqtt_client_connect(sock, &ci);
}

static void mqtt_publish(sock_tcp_t *sock)
{
    const char *message = (strcmp(MQTT_MESSAGE, "work") == 0) ? "not work" : "work";
    mqtt_publish(sock, MQTT_TOPIC, message, strlen(message), MQTT_QOS, MQTT_RETAIN);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return 1;
    }

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
