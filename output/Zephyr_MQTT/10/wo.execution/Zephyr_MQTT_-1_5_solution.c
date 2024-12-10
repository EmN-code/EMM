
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/random/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <sys/time.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;
struct mqtt_topic topic;
struct mqtt_binstr payload;

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT: Connected to broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT: Disconnected from broker\n");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_client *client)
{
    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    payload.data = MQTT_PAYLOAD;
    payload.size = strlen(MQTT_PAYLOAD);

    publish_message.topic = &topic;
    publish_message.payload = &payload;
    publish_message.message_id = 0; // QoS 0, no message ID needed
    publish_message.dup_flag = 0;
    publish_message.retain_flag = 0;

    publish_param.message = publish_message;
    publish_param.message_id = 0; // QoS 0, no message ID needed
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    mqtt_publish(client, &publish_param);
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("MQTT: Connection failed %d\n", ret);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
