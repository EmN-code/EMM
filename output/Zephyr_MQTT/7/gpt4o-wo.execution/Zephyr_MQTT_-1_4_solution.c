
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/slist.h>
#include <zephyr/linker/sections.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/syscalls/i2c.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;
struct sockaddr_in broker_addr;

void mqtt_publish_state(void)
{
    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = MQTT_PAYLOAD_LEN;

    publish_param.message = publish_message;
    publish_param.message_id = 0; // Not needed for QoS 0
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(&client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published successfully\n");
    }
}

void mqtt_connect_and_publish(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    int ret = connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    if (ret < 0) {
        printk("Failed to connect to broker: %d\n", ret);
        close(sock);
        return;
    }

    mqtt_client_init(&client);
    client.broker = &broker_addr;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        close(sock);
        return;
    }

    printk("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    printk("Starting MQTT client\n");
    mqtt_connect_and_publish();
    return 0;
}
