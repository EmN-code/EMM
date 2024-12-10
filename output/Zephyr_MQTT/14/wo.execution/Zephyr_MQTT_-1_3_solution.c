
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel_includes.h>
#include <time.h>
#include <sys/time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 1
#define MQTT_RETAIN false
#define MQTT_DUP false

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
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
        case MQTT_EVT_PUBLISH:
            printk("Publish event received\n");
            break;
        default:
            break;
    }
}

static void publish_state(struct k_work *work)
{
    static int state = 0;
    const char *payload = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic = topic;
    publish_param.message.payload.data = (uint8_t *)payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.message_id = 0;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(&client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", payload);
    }
}

K_WORK_DEFINE(publish_work, publish_state);

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&client, 0, sizeof(client));
    memset(&topic, 0, sizeof(topic));
    memset(&publish_param, 0, sizeof(publish_param));

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = true;
    client.keepalive = 60;
    client.will_topic = NULL;
    client.will_msg = NULL;
    client.will_qos = 0;
    client.will_retain = false;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        k_work_submit(&publish_work);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
