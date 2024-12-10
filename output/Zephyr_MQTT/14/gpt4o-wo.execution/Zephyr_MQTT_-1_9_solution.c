
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/devicetree.h>
#include <time.h>
#include <zephyr/posix/arpa/inet.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

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
    int ret;

    memset(&client, 0, sizeof(client));
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = 1;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        struct mqtt_publish_param param;
        struct mqtt_publish_message message;

        message.topic.utf8 = MQTT_TOPIC;
        message.topic.size = strlen(MQTT_TOPIC);
        message.payload.data = MQTT_MESSAGE;
        message.payload.len = strlen(MQTT_MESSAGE);

        param.message = message;
        param.message_id = 0; // QoS 0, no message ID needed
        param.dup_flag = 0;
        param.retain_flag = 0;

        ret = mqtt_publish(&client, &param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
