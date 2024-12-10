
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static struct mqtt_client client;
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];

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
    struct sockaddr_storage broker;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    int sock;

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)broker_addr, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = MQTT_RETAIN;

    while (1) {
        if (mqtt_publish(&client, &publish_param) != 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
    close(sock);
    return 0;
}
