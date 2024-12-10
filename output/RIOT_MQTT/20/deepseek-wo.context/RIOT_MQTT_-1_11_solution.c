
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void _mqtt_connect(mqtt_client_t *client, void *arg)
{
    (void)arg;
    printf("Connected to MQTT broker\n");
}

static void _mqtt_disconnect(mqtt_client_t *client, void *arg, mqtt_connection_status_t reason)
{
    (void)client;
    (void)arg;
    printf("Disconnected from MQTT broker: %d\n", reason);
}

static void _mqtt_publish(mqtt_client_t *client, void *arg, const char *topic, const char *payload, size_t len, int qos, int retain)
{
    (void)client;
    (void)arg;
    printf("Published to topic %s: %s\n", topic, payload);
}

int main(void)
{
    mqtt_client_t client;
    sock_tcp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_t broker_addr;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    if (inet_pton(AF_INET, BROKER_IP, &broker_addr) <= 0) {
        printf("Invalid broker IP address\n");
        return 1;
    }
    broker.addr.ipv4 = broker_addr;

    mqtt_client_config_t config = {
        .connect = _mqtt_connect,
        .disconnect = _mqtt_disconnect,
        .publish = _mqtt_publish,
        .arg = NULL
    };

    mqtt_client_init(&client, &config);

    if (mqtt_client_connect(&client, &broker) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        xtimer_usleep_until(&last_wakeup, INTERVAL);
        mqtt_client_publish(&client, TOPIC, PAYLOAD, strlen(PAYLOAD), 0, 0);
    }

    return 0;
}
