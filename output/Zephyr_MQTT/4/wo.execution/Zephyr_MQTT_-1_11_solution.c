
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/slist.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_select.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

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
    struct mqtt_connect_param connect_params;
    struct mqtt_publish_param publish_params;
    struct mqtt_topic topic;
    struct mqtt_utf8 client_id;
    char payload[20];

    // Initialize MQTT client
    mqtt_client_init(&client);

    // Set up MQTT client parameters
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    // Set up broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    // Set up client ID
    client_id.utf8 = MQTT_CLIENT_ID;
    client_id.size = strlen(MQTT_CLIENT_ID);
    client.client_id = client_id;

    // Connect to MQTT broker
    connect_params.client_id = client_id;
    connect_params.user_name = NULL;
    connect_params.password = NULL;
    connect_params.clean_session = MQTT_CLEAN_SESSION;
    connect_params.keepalive = MQTT_KEEPALIVE;
    connect_params.will_topic = NULL;
    connect_params.will_message = NULL;
    connect_params.will_qos = 0;
    connect_params.will_retain = 0;

    mqtt_connect(&client, &connect_params);

    // Publish state every 5 seconds
    while (1) {
        snprintf(payload, sizeof(payload), "work");

        topic.topic.utf8 = MQTT_TOPIC;
        topic.topic.size = strlen(MQTT_TOPIC);
        topic.qos = 0;

        publish_params.message.topic = topic;
        publish_params.message.payload.data = payload;
        publish_params.message.payload.size = strlen(payload);
        publish_params.dup_flag = 0;
        publish_params.retain_flag = 0;
        publish_params.message_id = 0;

        mqtt_publish(&client, &publish_params);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
