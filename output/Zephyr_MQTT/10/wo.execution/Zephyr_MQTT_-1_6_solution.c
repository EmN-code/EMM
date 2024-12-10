
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

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static struct mqtt_topic topic;
static struct mqtt_binstr payload;

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

int main(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_addr.s_addr = htonl(0x2F666767); // 47.102.103.1
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("Failed to connect to MQTT broker, error: %d\n", ret);
        return ret;
    }

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    payload.utf8 = MQTT_PAYLOAD;
    payload.size = strlen(MQTT_PAYLOAD);

    publish_message.topic = topic;
    publish_message.payload = payload;
    publish_message.payload_len = payload.size;

    publish_param.message = publish_message;
    publish_param.message_id = 0; // Not used for QoS 0
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    while (1) {
        ret = mqtt_publish(&client_ctx, &publish_param);
        if (ret) {
            printk("Failed to publish message, error: %d\n", ret);
        } else {
            printk("Message published\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
