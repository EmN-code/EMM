#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to the broker\n");
        break;
    case MQTT_EVT_PUBACK:
        printk("Message published\n");
        break;
    default:
        break;
    }
}

void connect_to_broker(void)
{
    struct sockaddr_in *broker4 = &broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    struct mqtt_client *client = &client;
    mqtt_client_init(client);

    client->broker = &broker;
    client->evt_cb = mqtt_evt_handler;
    client->client_id.utf8 = (const uint8_t *)"zephyr_client";
    client->client_id.size = strlen("zephyr_client");
    client->protocol_version = MQTT_VERSION_3_1_1;

    int rc = mqtt_connect(client);
    if (rc != 0) {
        printk("mqtt_connect failed: %d\n", rc);
        return;
    }
}

void main(void)
{
    connect_to_broker();

    while (1) {
        pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        pub_param.message.topic.topic.utf8 = (const uint8_t *)"zephyr/state";
        pub_param.message.topic.topic.size = strlen("zephyr/state");
        pub_param.message.payload.data = (const uint8_t *)"work or not work";
        pub_param.message.payload.len = strlen("work or not work");
        pub_param.message_id = sys_rand32_get();
        pub_param.dup_flag = 0U;
        pub_param.retain_flag = 0U;

        int rc = mqtt_publish(&client, &pub_param);
        if (rc != 0) {
            printk("mqtt_publish failed: %d\n", rc);
        }

        k_sleep(PUBLISH_INTERVAL);
    }
}