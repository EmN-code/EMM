
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void _mqtt_connect(sock_tcp_t *sock)
{
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void _mqtt_publish(sock_tcp_t *sock)
{
    MQTT_Publish pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_PAYLOAD;
    pub.payload_len = strlen(MQTT_PAYLOAD);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    if (MQTTPublish(sock, &pub) < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    if (sock_tcp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    _mqtt_connect(&sock);

    while (1) {
        _mqtt_publish(&sock);
        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
