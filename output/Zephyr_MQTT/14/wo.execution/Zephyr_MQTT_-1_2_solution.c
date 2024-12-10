
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_msg;

void mqtt_publish_work(struct k_work *work)
{
    publish_param.message.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = MQTT_PAYLOAD;
    publish_param.message.payload.len = strlen(MQTT_PAYLOAD);
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(&client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published successfully\n");
    }
}

K_WORK_DEFINE(publish_work, mqtt_publish_work);

void mqtt_connect_and_publish(void)
{
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");

    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    printk("Connected to MQTT broker\n");

    while (1) {
        k_work_submit(&publish_work);
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    printk("Starting MQTT client\n");

    mqtt_client_init(&client);
    mqtt_connect_and_publish();

    return 0;
}
