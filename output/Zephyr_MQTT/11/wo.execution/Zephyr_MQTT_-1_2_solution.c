
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/sys/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;

void mqtt_connect_and_publish(void) {
    struct sockaddr_in broker;
    int sock;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    mqtt_client_init(&client);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_MESSAGE;
    publish_message.payload.len = strlen(MQTT_MESSAGE);
    publish_message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    publish_message.retain = 0;

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    if (mqtt_publish(&client, &publish_param) != 0) {
        printk("Failed to publish message\n");
    } else {
        printk("Message published successfully\n");
    }

    mqtt_disconnect(&client);
    close(sock);
}

int main(void) {
    while (1) {
        mqtt_connect_and_publish();
        k_sleep(K_SECONDS(5));
    }
    return 0;
}
