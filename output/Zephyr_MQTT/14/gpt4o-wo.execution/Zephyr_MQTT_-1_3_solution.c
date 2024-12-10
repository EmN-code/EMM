
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/posix/arpa/inet.h>
#include <sys/time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/device.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/posix/poll.h>
#include <zephyr/kernel_includes.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

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
    int ret;
    struct mqtt_client_config config = {0};

    // Initialize MQTT client
    mqtt_client_init(&client);

    // Set up MQTT client configuration
    config.client_id = "zephyr_client";
    config.user_name = NULL;
    config.password = NULL;
    config.keep_alive = 60;
    config.clean_session = true;
    config.client_addr = (struct sockaddr *)&broker;
    config.client_addr_len = sizeof(broker);

    // Set up MQTT broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    // Set up MQTT publish message
    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);
    publish_message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    publish_message.retain = 0;

    // Set up MQTT publish parameters
    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    // Connect to MQTT broker
    ret = mqtt_connect(&client, &config, mqtt_evt_handler, NULL);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    // Publish state every 5 seconds
    while (1) {
        ret = mqtt_publish(&client, &publish_param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
