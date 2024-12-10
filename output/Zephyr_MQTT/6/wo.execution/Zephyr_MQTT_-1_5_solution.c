
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <stdio.h>
#include <string.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <mqtt_client_init>
#include <mqtt_connect>
#include <mqtt_disconnect>
#include <close>
#include <inet_pton>
#include <k_sleep>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <time.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/wifi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <sys/time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
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

int main(void)
{
    int sock;
    struct sockaddr_in broker;
    struct mqtt_connect_param conn_param;
    struct mqtt_publish_param pub_param;
    struct mqtt_topic topic;

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
    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    // Connect to MQTT broker
    if (connect(client.transport.tcp.sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(client.transport.tcp.sock);
        return -1;
    }

    // Set up MQTT client parameters
    client.client_id.utf8 = "esp32_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    // Connect to MQTT broker
    conn_param.client_id = client.client_id;
    conn_param.user_name = NULL;
    conn_param.password = NULL;
    conn_param.keepalive = client.keepalive;
    conn_param.clean_session = client.clean_session;
    conn_param.will_topic = NULL;
    conn_param.will_message = NULL;
    conn_param.will_qos = 0;
    conn_param.will_retain = 0;

    if (mqtt_connect(&client, &conn_param) != 0) {
        printk("Failed to connect to MQTT broker\n");
        close(client.transport.tcp.sock);
        return -1;
    }

    // Publish state every 5 seconds
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = 0;

    pub_param.message.topic = topic;
    pub_param.message.payload.data = MQTT_PAYLOAD;
    pub_param.message.payload.len = MQTT_PAYLOAD_SIZE;
    pub_param.message_id = 0;
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    while (1) {
        if (mqtt_publish(&client, &pub_param) != 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Published message: %s\n", MQTT_PAYLOAD);
        }
        k_sleep(K_SECONDS(5));
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);
    close(client.transport.tcp.sock);

    return 0;
}
