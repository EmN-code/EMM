
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
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

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_topic topic;
struct mqtt_utf8 payload;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
            } else {
                printk("Failed to connect to MQTT broker\n");
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
    int sock;
    struct sockaddr_in broker_addr;

    // Initialize MQTT client
    memset(&client, 0, sizeof(client));
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = malloc(512);
    client.rx_buf_size = 512;
    client.tx_buf = malloc(512);
    client.tx_buf_size = 512;
    client.keepalive = 60;
    client.clean_session = 1;

    // Initialize MQTT topic and payload
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    payload.utf8 = MQTT_PAYLOAD;
    payload.size = strlen(MQTT_PAYLOAD);

    // Initialize MQTT publish parameters
    publish_param.message.topic = &topic;
    publish_param.message.payload = &payload;
    publish_param.message.payload_size = payload.size;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    // Create socket and connect to MQTT broker
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Initialize MQTT transport
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    // Connect to MQTT broker
    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (mqtt_publish(&client, &publish_param) != 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
