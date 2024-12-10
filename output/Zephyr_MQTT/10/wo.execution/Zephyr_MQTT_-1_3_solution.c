
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <sys/time.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static struct sockaddr_in broker;

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
    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.utf8 = MQTT_PAYLOAD;
    publish_message.payload.size = strlen(MQTT_PAYLOAD);
    publish_message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    publish_message.retained = 0;

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published\n");
    }
}

int main(void)
{
    int ret;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    client.broker = (struct sockaddr *)broker_addr;
    client.broker_len = sizeof(broker);
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
