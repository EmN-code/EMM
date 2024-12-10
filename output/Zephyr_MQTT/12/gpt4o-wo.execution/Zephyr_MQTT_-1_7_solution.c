
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <sys/time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LENGTH 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct sockaddr_in broker;
static int sock;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                printk("Failed to connect to MQTT broker: %d\n", evt->result);
            } else {
                printk("Connected to MQTT broker\n");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_client *client)
{
    static uint16_t message_id = 0;
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = MQTT_PAYLOAD;
    message.payload.len = MQTT_PAYLOAD_LENGTH;

    param.message = message;
    param.message_id = message_id++;
    param.dup_flag = MQTT_DUP;
    param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published\n");
    }
}

void main(void)
{
    struct mqtt_client_config config;
    struct mqtt_client_connect_config connect_config;

    memset(&client, 0, sizeof(client));
    memset(&config, 0, sizeof(config));
    memset(&connect_config, 0, sizeof(connect_config));

    config.client_id.utf8 = "zephyr_client";
    config.client_id.size = strlen(config.client_id.utf8);
    config.user_name.utf8 = NULL;
    config.user_name.size = 0;
    config.password.utf8 = NULL;
    config.password.size = 0;

    connect_config.keepalive = 60;
    connect_config.clean_session = 1;
    connect_config.will_message = NULL;
    connect_config.will_topic = NULL;
    connect_config.will_qos = 0;
    connect_config.will_retain = 0;

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_cfg = &config;
    client.connect_cfg = &connect_config;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to broker: %d\n", errno);
        close(sock);
        return;
    }

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    mqtt_client_init(&client);

    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        close(sock);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }
}
