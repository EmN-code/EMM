
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/devicetree.h>
#include <zephyr/posix/arpa/inet.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

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
    struct sockaddr_in broker;
    int sock;

    // Initialize MQTT client
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = 1;

    // Set up broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    // Connect to MQTT broker
    mqtt_connect(&client);

    while (1) {
        // Publish state every 5 seconds
        struct mqtt_publish_param param;
        param.message.topic.topic.utf8 = MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = MQTT_PAYLOAD;
        param.message.payload.len = MQTT_PAYLOAD_SIZE;
        param.message.qos = MQTT_QOS;
        param.message.retained = 0;
        param.dup_flag = 0;
        param.retain_flag = 0;

        mqtt_publish(&client, &param);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
