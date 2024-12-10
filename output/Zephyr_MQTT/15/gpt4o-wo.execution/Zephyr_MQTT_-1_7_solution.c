#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/types.h>
#include <zephyr/net/mqtt.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "zephyr_client"
#define MQTT_TOPIC "device/state"

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected
");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected
");
            break;
        default:
            break;
    }
}

int main(void)
{
    struct sockaddr_in broker;
    struct mqtt_client client;
    uint8_t rx_buffer[128];
    uint8_t tx_buffer[128];
    struct mqtt_publish_param param;

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    while (1) {
        if (mqtt_connect(&client) != 0) {
            printk("Failed to connect to MQTT broker
");
            k_sleep(K_SECONDS(5));
            continue;
        }

        const char *message = "work";
        param.message.payload.data = message;
        param.message.payload.len = strlen(message);
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message_id = sys_rand32_get();
        param.dup_flag = 0;
        param.retain_flag = 0;

        while (1) {
            if (mqtt_publish(&client, &param)) {
                printk("Failed to publish message
");
                break;
            }
            printk("Published message: %s
", message);
            k_sleep(K_SECONDS(5));
        }

        mqtt_disconnect(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}