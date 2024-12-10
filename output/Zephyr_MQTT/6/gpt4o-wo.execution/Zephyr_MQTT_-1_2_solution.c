
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/wifi.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

void mqtt_publish_state(struct mqtt_client *client) {
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = MQTT_PAYLOAD;
    message.payload.len = strlen(MQTT_PAYLOAD);
    message.qos = MQTT_QOS_0_AT_MOST_ONCE;

    param.message = message;
    param.message_id = 0; // Not used for QoS 0
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

int main(void) {
    struct mqtt_client client;
    struct sockaddr_in broker_addr;
    int sock;

    // Initialize the MQTT client
    memset(&client, 0, sizeof(client));
    client.broker = &broker_addr;
    client.broker_len = sizeof(broker_addr);

    // Set up the broker address
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Connect to the MQTT broker
    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    // Set the socket for the MQTT client
    client.socket = sock;

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
