
#include <zephyr/net/net_mgmt.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/device.h>
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
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 1

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const c,
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
    int sock;
    struct mqtt_connect_param connect_param;

    // Initialize MQTT client
    memset(&client, 0, sizeof(client));
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    // Initialize MQTT connect parameters
    connect_param.keepalive = 60;
    connect_param.clean_session = 1;
    connect_param.client_id = client.client_id;
    connect_param.user_name.utf8 = NULL;
    connect_param.user_name.size = 0;
    connect_param.password.utf8 = NULL;
    connect_param.password.size = 0;

    // Initialize MQTT publish parameters
    publish_message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.utf8 = (uint8_t *)MQTT_PAYLOAD;
    publish_message.payload.size = strlen(MQTT_PAYLOAD);

    publish_param.message = publish_message;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    // Initialize socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Initialize broker address
    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    // Connect to MQTT broker
    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    // Connect MQTT client
    if (mqtt_connect(&client, &connect_param) < 0) {
        printk("Failed to connect MQTT client\n");
        close(sock);
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (mqtt_publish(&client, &publish_param) < 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Published message: %s\n", MQTT_PAYLOAD);
        }
        k_sleep(K_SECONDS(5));
    }

    // Disconnect MQTT client
    mqtt_disconnect(&client);

    // Close socket
    close(sock);

    return 0;
}
