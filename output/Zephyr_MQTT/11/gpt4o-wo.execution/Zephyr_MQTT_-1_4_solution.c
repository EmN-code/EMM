
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/socket_poll.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
            } else {
                printk("Failed to connect to MQTT broker\n");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

void publish_state(struct k_work *work)
{
    static int state = 0;
    const char *payload = state ? "work" : "not work";
    state = !state;

    struct mqtt_publish_message message = {
        .topic = {
            .topic.utf8 = MQTT_TOPIC,
            .topic.size = strlen(MQTT_TOPIC)
        },
        .payload.data = payload,
        .payload.len = strlen(payload)
    };

    publish_param.message = message;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(&client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", payload);
    }
}

K_WORK_DEFINE(publish_work, publish_state);

void main(void)
{
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    k_work_init(&publish_work, publish_state);
    k_work_submit_to_queue(&k_sys_work_q, &publish_work);

    while (1) {
        k_sleep(K_SECONDS(5));
        k_work_submit_to_queue(&k_sys_work_q, &publish_work);
    }
}
