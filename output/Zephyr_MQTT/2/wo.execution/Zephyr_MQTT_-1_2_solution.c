
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/storage/flash_map.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define PUBLISH_INTERVAL 5000

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;

void mqtt_publish_state(struct mqtt_client *client) {
    publish_message.topic.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.topic.size = sizeof(MQTT_TOPIC) - 1;
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.size = MQTT_PAYLOAD_LEN;

    publish_param.message = publish_message;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    mqtt_publish(client, &publish_param);
}

void main(void) {
    mqtt_client_init(&client);

    // Initialize MQTT client and connect to broker
    client.broker.hostname.utf8 = MQTT_BROKER_IP;
    client.broker.hostname.size = sizeof(MQTT_BROKER_IP) - 1;
    client.broker.port = MQTT_BROKER_PORT;

    mqtt_connect(&client);

    while (1) {
        mqtt_publish_state(&client);
        k_sleep(K_MSEC(PUBLISH_INTERVAL));
    }
}
