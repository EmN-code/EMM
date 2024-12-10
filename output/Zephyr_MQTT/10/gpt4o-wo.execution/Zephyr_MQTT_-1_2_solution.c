
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/random/random.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/iterable_sections.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL 5000  // 5 seconds

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 client_id = {
    .utf8 = "zephyr_client",
    .size = 13
};

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
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

void publish_state(struct mqtt_client *client, const char *message)
{
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = message;
    param.message.payload.len = strlen(message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", message);
    }
}

void main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    memset(&client_ctx, 0, sizeof(client_ctx));
    client_ctx.broker = &broker;
    client_ctx.client_id = &client_id;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    while (1) {
        publish_state(&client_ctx, MQTT_MESSAGE);
        k_sleep(K_MSEC(PUBLISH_INTERVAL));
        publish_state(&client_ctx, MQTT_MESSAGE_NOT_WORK);
        k_sleep(K_MSEC(PUBLISH_INTERVAL));
    }
}
