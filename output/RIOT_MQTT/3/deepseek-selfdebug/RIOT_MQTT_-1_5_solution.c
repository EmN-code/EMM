
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void _mqtt_connect(sock_tcp_t *sock)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(sock, &remote, &local, 0) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void _mqtt_publish(sock_tcp_t *sock)
{
    mqtt_publish_t pub;
    pub.topic = TOPIC;
    pub.payload = PAYLOAD;
    pub.payload_len = PAYLOAD_LEN;
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    if (mqtt_publish(sock, &pub) < 0) {
        puts("Error: could not publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    _mqtt_connect(&sock);

    while (1) {
        _mqtt_publish(&sock);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
