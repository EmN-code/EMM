
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/poll.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/devicetree.h>
#include <time.h>
#include <zephyr/random/random.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

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
    int sock;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    mqtt_client_init(&client);
    client.broker = &broker;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        struct mqtt_publish_param param;
        struct mqtt_publish_message message;

        message.topic.utf8 = MQTT_TOPIC;
        message.topic.size = strlen(MQTT_TOPIC);
        message.payload.data = MQTT_PAYLOAD;
        message.payload.len = strlen(MQTT_PAYLOAD);

        param.message = message;
        param.message_id = 1;
        param.dup_flag = 0;
        param.retain_flag = 0;

        if (mqtt_publish(&client, &param) != 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
