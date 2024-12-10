
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

static void publish_state(void)
{
    mqtt_publish(&mqtt_client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
}

static void connect_to_broker(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&mqtt_sock, &local, &remote, 0);
    mqtt_client_init(&mqtt_client, &mqtt_sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect(&mqtt_client, MQTT_CLIENT_ID, NULL, NULL, 0, MQTT_KEEPALIVE);
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
