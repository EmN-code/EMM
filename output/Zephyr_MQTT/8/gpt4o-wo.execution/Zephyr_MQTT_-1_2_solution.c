
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/printk.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_tx_buf tx_buf;
static struct mqtt_rx_buf rx_buf;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to MQTT broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    default:
        break;
    }
}

void publish_state(struct mqtt_client *client)
{
    int ret;
    struct mqtt_pub_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = 0;

    ret = mqtt_publish(client, &param);
    if (ret) {
        printk("Failed to publish message, error: %d\n", ret);
    } else {
        printk("Message published\n");
    }
}

void main(void)
{
    int ret;
    struct sockaddr_in broker;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    tx_buf.buf = tx_buffer;
    tx_buf.len = sizeof(tx_buffer);
    rx_buf.buf = rx_buffer;
    rx_buf.len = sizeof(rx_buffer);

    client_ctx.tx_buf = &tx_buf;
    client_ctx.rx_buf = &rx_buf;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("Failed to connect to MQTT broker, error: %d\n", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
