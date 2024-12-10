
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
#define MQTT_KEEPALIVE 360
#define MQTT_CLIENT_ID "riot_esp32"

static sock_tcp_t sock;
static mqtt_client_t client;

static void _mqtt_connect(void)
{
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    mqtt_init(&client, &sock);
    mqtt_connect(&client, MQTT_CLIENT_ID, NULL, NULL, MQTT_KEEPALIVE);
}

static void _mqtt_publish(void)
{
    mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
}

int main(void)
{
    _mqtt_connect();

    while (1) {
        _mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
