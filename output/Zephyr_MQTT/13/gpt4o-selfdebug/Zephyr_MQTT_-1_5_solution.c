#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    case MQTT_EVT_PUBLISH:
        printk("Message published to topic");
        break;
    default:
        break;
    }
}

void main(void)
{
    struct mqtt_publish_param param;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    net_addr_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client) != 0) {
        printk("MQTT connect failed");
        return;
    }

    while (1) {
        k_sleep(PUBLISH_INTERVAL);

        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = (uint8_t *)"device/state";
        param.message.topic.topic.size = strlen("device/state");
        param.message.payload.data = (uint8_t *)"work";
        param.message.payload.len = strlen("work");
        param.message_id = 0;
        param.dup_flag = 0;
        param.retain_flag = 0;

        mqtt_publish(&client, &param);
    }
}