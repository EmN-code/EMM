
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
            } else {
                printk("Failed to connect to MQTT broker: %d\n", evt->result);
            }
            break;

        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;

        default:
            break;
    }
}

int main(void)
{
    struct sockaddr_in broker;
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = -1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        k_sleep(K_SECONDS(5));

        message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        message.topic.size = strlen(MQTT_TOPIC);
        message.payload.data = (uint8_t *)MQTT_MESSAGE;
        message.payload.len = strlen(MQTT_MESSAGE);
        message.qos = MQTT_QOS_0_AT_MOST_ONCE;
        message.retain = 0;

        param.message = message;
        param.message_id = 0;
        param.dup_flag = 0;
        param.retain_flag = 0;

        ret = mqtt_publish(&client, &param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published\n");
        }
    }

    return 0;
}
