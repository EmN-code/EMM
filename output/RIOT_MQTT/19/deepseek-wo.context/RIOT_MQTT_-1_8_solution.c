
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void _mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port)
{
    sock_tcp_ep_t broker = { .family = AF_INET, .port = broker_port };
    inet_pton(AF_INET, broker_ip, &broker.addr.ipv4);

    mqtt_client_connect(client, &broker, NULL, 0, NULL, 0, 0, 0);
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *payload, size_t payload_len)
{
    mqtt_publish(client, topic, payload, payload_len, 0, 0, NULL, NULL);
}

int main(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client, NULL, 0, NULL, 0);

    _mqtt_connect(&client, BROKER_IP, BROKER_PORT);

    while (1) {
        _mqtt_publish(&client, TOPIC, PAYLOAD, PAYLOAD_LEN);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
