
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <sys/time.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/random/random.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/mqtt_sn.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
            } else {
                printk("Failed to connect to MQTT broker\n");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("Publish event received\n");
            break;
        case MQTT_EVT_PUBACK:
            printk("Publish acknowledgment received\n");
            break;
        default:
            break;
    }
}

void publish_state(struct k_work *work)
{
    static bool state = false;
    const char *payload = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = (uint8_t *)payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.message.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.retain = 0;

    int ret = mqtt_publish(&client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", payload);
    }
}

K_WORK_DEFINE(publish_work, publish_state);

void main(void)
{
    printk("Starting MQTT client\n");

    mqtt_client_init(&client);
    client.broker.utf8 = MQTT_BROKER_ADDR;
    client.broker.size = strlen(MQTT_BROKER_ADDR);
    client.broker_port = MQTT_BROKER_PORT;
    client.evt_cb = mqtt_evt_handler;

    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        k_work_submit(&publish_work);
    }
}
