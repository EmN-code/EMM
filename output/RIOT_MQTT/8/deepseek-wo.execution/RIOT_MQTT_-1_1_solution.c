
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

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
    inet_pton(AF_INET, broker_ip, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_client_init(client, &sock, NULL, 0, NULL, 0);
    mqtt_connect(client, MQTT_CLIENT_ID, NULL, NULL, 0, MQTT_QOS, MQTT_RETAIN);
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *message)
{
    mqtt_publish(client, topic, message, strlen(message), MQTT_QOS, MQTT_RETAIN);
}

int main(void)
{
    mqtt_client_t client;

    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Connect to MQTT broker
    _mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Publish state every 5 seconds
    while (1) {
        _mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
