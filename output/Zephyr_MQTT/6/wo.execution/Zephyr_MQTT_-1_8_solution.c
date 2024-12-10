
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <stdio.h>
#include <string.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <mqtt_client_init.h>
#include <mqtt_connect.h>
#include <mqtt_disconnect.h>
#include <close.h>
#include <inet_pton.h>
#include <k_sleep.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <time.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/wifi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const client,
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
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = 1;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);
    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        struct mqtt_publish_param param;
        struct mqtt_topic topic = {
            .topic.utf8 = MQTT_TOPIC,
            .topic.size = strlen(MQTT_TOPIC),
            .qos = MQTT_QOS_1_AT_LEAST_ONCE
        };

        param.message.topic = &topic;
        param.message.payload.data = state;
        param.message.payload.len = strlen(state);
        param.message_id = 1;
        param.dup_flag = 0;
        param.retain_flag = 0;

        ret = mqtt_publish(&client, &param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Published: %s\n", state);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
