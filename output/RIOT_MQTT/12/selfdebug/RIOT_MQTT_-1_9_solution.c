#include <xtimer.h>
#include <msg.h>
#include <net/mqtt.h>
#include <net/emcute.h>  // You can consider this as the underlying library for networking support.

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/status"

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static mqtt_client_t mqtt_client;
static mqtt_client_config_t mqtt_config;

static msg_t msg_queue[8];

void configure_mqtt(mqtt_client_t *client) {
    memset(client, 0, sizeof(*client));
    client->iface = NETDEV_IPV6;  // Use appropriate interface
    client->port = MQTT_BROKER_PORT;
    client->server.addr.ipv4 = inet_addr(MQTT_BROKER_IP);
    client->server.port = MQTT_BROKER_PORT;
    client->keep_alive = 60;
    client->client_id = "esp32_device";
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    configure_mqtt(&mqtt_client);
    
    mqtt_connect(&mqtt_client);

    while (1) {
        const char *state = "work";  // Modify state as needed
        mqtt_publish(&mqtt_client, MQTT_TOPIC, state, strlen(state), MQTT_QOS_0);
        xtimer_sleep(INTERVAL);
    }
    return 0;
}