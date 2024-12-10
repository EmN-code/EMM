
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <sys/time.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/net/socket_poll.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_connect_param connect_param;
static struct mqtt_publish_message publish_message;
static struct mqtt_topic topic;

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    int ret;
    struct sockaddr_storage broker;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    memset(&client, 0, sizeof(client));
    memset(&publish_param, 0, sizeof(publish_param));
    memset(&connect_param, 0, sizeof(connect_param));
    memset(&publish_message, 0, sizeof(publish_message));
    memset(&topic, 0, sizeof(topic));

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = 1;
    client.keepalive = 60;
    client.will_topic = NULL;
    client.will_message = NULL;
    client.will_qos = 0;
    client.will_retain = 0;

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);

    connect_param.keepalive = client.keepalive;
    connect_param.clean_session = client.clean_session;
    connect_param.client_id = client.client_id;
    connect_param.user_name = client.user_name;
    connect_param.password = client.password;
    connect_param.will_topic = client.will_topic;
    connect_param.will_message = client.will_message;
    connect_param.will_qos = client.will_qos;
    connect_param.will_retain = client.will_retain;

    ret = mqtt_connect(&client, &connect_param);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS;

    publish_message.topic = topic;
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    while (1) {
        ret = mqtt_publish(&client, &publish_param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published successfully!\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
