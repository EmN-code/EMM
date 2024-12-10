
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <time.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/coap.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/device_mmio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

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

int connect_mqtt(void)
{
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = MQTT_CLEAN_SESSION;
    client.keepalive = MQTT_KEEPALIVE;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    return mqtt_connect(&client);
}

int publish_state(void)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
        .qos = MQTT_QOS_1_AT_LEAST_ONCE
    };
    struct mqtt_binstr payload = {
        .data = (uint8_t *)MQTT_PAYLOAD,
        .len = MQTT_PAYLOAD_SIZE
    };

    param.message.topic = topic;
    param.message.payload = payload;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(&client, &param);
}

int main(void)
{
    int ret;

    ret = connect_mqtt();
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        ret = publish_state();
        if (ret) {
            printk("Failed to publish state: %d\n", ret);
        } else {
            printk("State published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
