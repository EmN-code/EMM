
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
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define INTERVAL_MS (5 * 1000U)

static void mqtt_connect(struct mqtt_client *client, const char *broker_ip, uint16_t broker_port)
{
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(broker_port);
    inet_pton(AF_INET, broker_ip, &broker_addr.sin_addr);

    mqtt_client_init(client);
    mqtt_client_connect(client, (struct sockaddr *)&broker_addr, sizeof(broker_addr), MQTT_KEEPALIVE);
}

static void mqtt_publish(struct mqtt_client *client, const char *topic, const char *message)
{
    mqtt_client_publish(client, topic, message, strlen(message), MQTT_QOS, MQTT_RETAIN);
}

int main(void)
{
    struct mqtt_client client;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
    }

    return 0;
}
