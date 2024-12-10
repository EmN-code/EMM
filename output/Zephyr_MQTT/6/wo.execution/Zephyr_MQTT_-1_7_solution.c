
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
#include <mqtt_client_init.h>
#include <mqtt_connect.h>
#include <mqtt_disconnect.h>
#include <close.h>
#include <inet_pton.h>
#include <k_sleep.h>
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
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"

struct mqtt_client client;

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

void publish_state(struct mqtt_client *client, const char *state)
{
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = state;
    message.payload.len = strlen(state);
    message.qos = MQTT_QOS_0_AT_MOST_ONCE;

    param.message = message;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    mqtt_client_init(&client);

    client.broker = &broker;
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.evt_cb = mqtt_evt_handler;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        publish_state(&client, "work");
        k_sleep(K_SECONDS(5));
        publish_state(&client, "not work");
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);

    return 0;
}
