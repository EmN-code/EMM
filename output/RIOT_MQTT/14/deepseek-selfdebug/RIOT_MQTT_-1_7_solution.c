
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"

static void mqtt_connect(sock_tcp_t *sock)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(sock, &remote, &local, 0);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
}

static void mqtt_publish(sock_tcp_t *sock, const char *topic, const char *message)
{
    mqtt_publish_t pub;
    pub.topic = topic;
    pub.payload = message;
    pub.payload_len = strlen(message);
    pub.qos = 0;
    pub.retain = 0;

    mqtt_client_t client;
    mqtt_client_init(&client, sock);
    mqtt_client_connect(&client, MQTT_CLIENT_ID, NULL, NULL, 0, NULL, 0, 0, 0);
    mqtt_client_publish(&client, &pub);
    mqtt_client_disconnect(&client);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
