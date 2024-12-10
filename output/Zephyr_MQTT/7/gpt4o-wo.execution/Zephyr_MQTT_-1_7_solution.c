
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <zephyr/linker/sections.h>
#include <time.h>
#include <zephyr/device.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
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

int mqtt_publish_state(struct mqtt_client *client, const char *state)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = &topic;
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = 0; // QoS 0
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(client, &param);
}

int main(void)
{
    struct sockaddr_in broker_addr;
    int sock;

    // Initialize MQTT client
    mqtt_client_init(&client);

    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set broker address
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    // Connect to MQTT broker
    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    // Set socket for MQTT client
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    // Connect MQTT client
    if (mqtt_connect(&client) < 0) {
        printk("Failed to connect MQTT client\n");
        close(sock);
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish_state(&client, "work");
        k_sleep(K_SECONDS(5));
        mqtt_publish_state(&client, "not work");
        k_sleep(K_SECONDS(5));
    }

    // Disconnect MQTT client
    mqtt_disconnect(&client);
    close(sock);

    return 0;
}
