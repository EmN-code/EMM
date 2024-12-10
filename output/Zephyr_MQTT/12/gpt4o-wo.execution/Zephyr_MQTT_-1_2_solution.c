
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/device.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_connect_param connect_param;
static struct mqtt_publish_message publish_message;

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected\n");
            break;
        default:
            break;
    }
}

void mqtt_publish_state(void)
{
    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_MESSAGE;
    publish_message.payload.len = strlen(MQTT_MESSAGE);
    publish_message.qos = MQTT_QOS;
    publish_message.retain = MQTT_RETAIN;
    publish_message.dup = MQTT_DUP;

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    mqtt_publish(&client, &publish_param);
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&client, 0, sizeof(client));
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
    client.will_msg = NULL;
    client.will_qos = 0;
    client.will_retain = 0;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
