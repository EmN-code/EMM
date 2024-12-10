
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static void mqtt_connect(sock_tcp_t *sock)
{
    sock_tcp_ep_t remote = { .family = AF_INET };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    remote.port = MQTT_BROKER_PORT;

    if (sock_tcp_connect(sock, &remote) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(sock_tcp_t *sock)
{
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = strlen(MQTT_PAYLOAD);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    if (mqtt_publish(sock, &pub) < 0) {
        puts("Error: failed to publish message");
        return;
    }

    puts("Message published");
}

int main(void)
{
    sock_tcp_t sock;

    puts("Starting MQTT client");

    mqtt_connect(&sock);

    while (1) {
        mqtt_publish(&sock);
        xtimer_sleep(5);
    }

    return 0;
}
