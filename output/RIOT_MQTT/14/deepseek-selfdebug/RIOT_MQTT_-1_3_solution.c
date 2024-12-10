
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void _mqtt_connect(struct mqtt_client *client, const char *broker_ip, uint16_t broker_port)
{
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(broker_port);
    inet_pton(AF_INET, broker_ip, &broker_addr.sin_addr);

    mqtt_client_init(client);
    mqtt_client_connect(client, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
}

static void _mqtt_publish(struct mqtt_client *client, const char *topic, const char *message)
{
    mqtt_publish(client, topic, message, strlen(message), 0, 0);
}

int main(void)
{
    struct mqtt_client client;

    _mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        _mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
