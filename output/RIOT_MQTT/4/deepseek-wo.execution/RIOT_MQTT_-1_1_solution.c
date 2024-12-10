
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void _mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = broker_port };
    inet_addr_from_str(remote.addr.ipv4, broker_ip);

    mqtt_connect(client, &remote, &local, MQTT_CLIENT_ID, NULL, NULL, 0);
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish(client, topic, message, strlen(message), MQTT_QOS, MQTT_RETAIN);
}

int main(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    _mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        _mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
