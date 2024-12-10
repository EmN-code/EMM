
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <time.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/coap.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/device_mmio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const c,
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
    default:
        break;
    }
}

int main(void)
{
    struct sockaddr_in broker;
    struct mqtt_connect_param conn_param;
    struct mqtt_publish_param pub_param;
    struct mqtt_publish_message pub_msg;
    struct mqtt_topic topic;
    struct mqtt_binstr payload;

    // Initialize MQTT client
    mqtt_client_init(&client);

    // Set up MQTT client
    client.broker = &broker;
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    // Set up broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    // Connect to MQTT broker
    conn_param.client_id = &client.client_id;
    conn_param.user_name = NULL;
    conn_param.password = NULL;
    conn_param.keepalive = 60;
    conn_param.clean_session = true;
    conn_param.will_topic = NULL;
    conn_param.will_message = NULL;
    conn_param.will_qos = 0;
    conn_param.will_retain = false;

    int ret = mqtt_connect(&client, &conn_param);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    // Publish state every 5 seconds
    while (1) {
        // Prepare publish message
        topic.topic.utf8 = MQTT_TOPIC;
        topic.topic.size = strlen(MQTT_TOPIC);
        topic.qos = 0;

        payload.data = (uint8_t *)MQTT_PAYLOAD_WORK;
        payload.size = strlen(MQTT_PAYLOAD_WORK);

        pub_msg.topic = topic;
        pub_msg.payload = payload;

        pub_param.message = pub_msg;
        pub_param.message_id = 0;
        pub_param.dup_flag = 0;
        pub_param.retain_flag = 0;

        ret = mqtt_publish(&client, &pub_param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Published message: %s\n", MQTT_PAYLOAD_WORK);
        }

        k_sleep(K_SECONDS(5));

        // Alternate between "work" and "not work"
        payload.data = (uint8_t *)MQTT_PAYLOAD_NOT_WORK;
        payload.size = strlen(MQTT_PAYLOAD_NOT_WORK);

        pub_msg.payload = payload;

        pub_param.message = pub_msg;

        ret = mqtt_publish(&client, &pub_param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Published message: %s\n", MQTT_PAYLOAD_NOT_WORK);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
