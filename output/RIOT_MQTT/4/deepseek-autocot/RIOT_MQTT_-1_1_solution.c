
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void _mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = broker_port };
    inet_pton(AF_INET, broker_ip, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_client_init(client, &sock, NULL, 0, NULL, 0);
    mqtt_connect(client, NULL, 0, NULL, 0);
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *payload, size_t payload_len)
{
    mqtt_publish(client, topic, payload, payload_len, 0, 0, NULL, NULL);
}

int main(void)
{
    mqtt_client_t client;

    puts("Connecting to MQTT broker...");
    _mqtt_connect(&client, BROKER_IP, BROKER_PORT);

    while (1) {
        puts("Publishing state...");
        _mqtt_publish(&client, TOPIC, PAYLOAD, PAYLOAD_LEN);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
