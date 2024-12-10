
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <time.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/device.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_topic topic;

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

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    message.topic = topic;
    message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    message.payload.len = strlen(MQTT_PAYLOAD);

    param.message = message;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

int main(void)
{
    struct sockaddr_in broker;
    int sock;

    // Initialize MQTT client
    mqtt_client_init(&client);

    // Set up MQTT transport
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client.transport.tcp.sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    // Connect to MQTT broker
    if (connect(client.transport.tcp.sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(client.transport.tcp.sock);
        return -1;
    }

    // Set up MQTT client parameters
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.keepalive = 60;
    client.clean_session = 1;

    // Initialize MQTT topic
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);

    // Connect to MQTT broker
    mqtt_connect(&client);

    // Publish state every 5 seconds
    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
