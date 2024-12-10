
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
#include <mqtt_client_init>
#include <mqtt_connect>
#include <mqtt_disconnect>
#include <close>
#include <inet_pton>
#include <k_sleep>
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
#include <zephyr/net/socket_select.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];

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
    struct mqtt_connect_param connect_params;
    struct mqtt_publish_param publish_params;
    struct mqtt_publish_message publish_msg;
    char payload[20];

    memset(&client, 0, sizeof(client));
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    connect_params.keepalive = 60;
    connect_params.clean_session = true;
    connect_params.client_id = &client.client_id;
    connect_params.user_name = NULL;
    connect_params.password = NULL;
    connect_params.will_topic = NULL;
    connect_params.will_message = NULL;

    mqtt_connect(&client, &connect_params);

    while (1) {
        k_sleep(K_SECONDS(5));

        snprintf(payload, sizeof(payload), "work");
        publish_msg.topic.utf8 = MQTT_TOPIC;
        publish_msg.topic.size = strlen(MQTT_TOPIC);
        publish_msg.payload.data = payload;
        publish_msg.payload.len = strlen(payload);
        publish_msg.qos = MQTT_QOS_0_AT_MOST_ONCE;
        publish_msg.retain = false;

        publish_params.message = publish_msg;
        publish_params.dup_flag = false;
        publish_params.retain_flag = false;
        publish_params.message_id = 0;

        mqtt_publish(&client, &publish_params);
    }

    return 0;
}
