
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
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(broker_port);
    inet_pton(AF_INET, broker_ip, &broker_addr.sin_addr);

    mqtt_connect(client, (struct sockaddr *)&broker_addr, NULL);
}

static void _mqtt_publish(mqtt_client_t *client, const char *topic, const char *payload, uint8_t qos)
{
    mqtt_publish(client, NULL, topic, payload, strlen(payload), qos, 0, NULL, NULL);
}

int main(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    _mqtt_connect(&client, BROKER_IP, BROKER_PORT);

    while (1) {
        _mqtt_publish(&client, TOPIC, PAYLOAD, 0);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
