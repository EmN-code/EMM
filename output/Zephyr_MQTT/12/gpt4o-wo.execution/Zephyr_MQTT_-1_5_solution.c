
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_connect_param connect_param;
static struct mqtt_publish_message publish_message;
static struct mqtt_topic topic;

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
    int ret;
    struct sockaddr_in broker_addr;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&connect_param, 0, sizeof(connect_param));
    memset(&publish_param, 0, sizeof(publish_param));
    memset(&publish_message, 0, sizeof(publish_message));
    memset(&topic, 0, sizeof(topic));

    client_ctx.broker = &broker_addr;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "esp32_client";
    client_ctx.client_id.size = strlen("esp32_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    connect_param.keepalive = 60;
    connect_param.clean_session = 1;
    connect_param.client_id = &client_ctx.client_id;
    connect_param.user_name = client_ctx.user_name;
    connect_param.password = client_ctx.password;
    connect_param.will_topic = NULL;
    connect_param.will_message = NULL;
    connect_param.will_qos = 0;
    connect_param.will_retain = 0;

    ret = mqtt_connect(&client_ctx, &connect_param);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    publish_message.topic = &topic;
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    while (1) {
        ret = mqtt_publish(&client_ctx, &publish_param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
