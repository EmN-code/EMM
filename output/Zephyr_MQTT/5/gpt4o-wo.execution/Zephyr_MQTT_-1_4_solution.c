#include <zephyr/sys/util.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/random/random.h>
#include <sys/time.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/fs/nvs.h>

#define MQTT_BROKER_ADDR   "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define MQTT_CLIENT_ID     "esp32"
#define MQTT_TOPIC         "esp32/state"

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *event) {
    switch (event->type) {
        case MQTT_EVT_CONNECT:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("MQTT message published!\n");
            break;
        default:
            break;
    }
}

int create_mqtt_client(struct mqtt_client *client) {
    struct sockaddr_in broker;

    mqtt_client_init(client);

    // Set broker details
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    // Assign broker address and prepare MQTT client
    client->broker = &broker;
    client->evt_cb = mqtt_event_handler;
    client->client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client->client_id.size = strlen(MQTT_CLIENT_ID);
    client->transport.type = MQTT_TRANSPORT_NON_SECURE;

    return mqtt_connect(client);
}

void main(void) {
    struct mqtt_client client;
    struct mqtt_publish_param param;
    int ret;

    // Set device state
    const char *state_msg = "work";

    // Create and connect MQTT client
    if (create_mqtt_client(&client) < 0) {
        printk("Failed to create and connect MQTT client\n");
        return;
    }

    while (1) {
        // Prepare the MQTT message
        param.message.payload.data = (uint8_t *)state_msg;
        param.message.payload.len = strlen(state_msg);
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message_id = sys_rand32_get();

        // Publish the message
        ret = mqtt_publish(&client, &param);
        if (ret) {
            printk("Failed to publish message [%d]\n", ret);
        }

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }
}