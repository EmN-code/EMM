
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/random/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <sys/time.h>
#include <zephyr/syscalls/device.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static struct mqtt_topic topic;
static struct mqtt_binstr payload;

void mqtt_evt_handler(struct mqtt_client *const c,
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
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    payload.data.utf8 = MQTT_PAYLOAD;
    payload.data.size = strlen(MQTT_PAYLOAD);

    publish_message.topic = &topic;
    publish_message.payload = &payload;
    publish_message.message_id = 0; // Not needed for QoS 0
    publish_message.dup_flag = 0;
    publish_message.retain_flag = MQTT_RETAIN;

    publish_param.message = publish_message;
    publish_param.message_id = 0; // Not needed for QoS 0
    publish_param.dup_flag = 0;
    publish_param.retain_flag = MQTT_RETAIN;

    mqtt_publish(client, &publish_param);
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&client, 0, sizeof(client));
    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
