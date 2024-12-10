
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/net_ip.h>
#include <sys/time.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/linker/sections.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/device_mmio.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_SIZE 15

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_topic topic;
struct mqtt_binstr payload;

void publish_state(struct k_work *work) {
    static bool state = false;
    char payload_str[MQTT_PAYLOAD_SIZE];

    state = !state;
    snprintf(payload_str, MQTT_PAYLOAD_SIZE, "%s", state ? "work" : "not work");

    payload.data = payload_str;
    payload.len = strlen(payload_str);

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);

    publish_param.message.topic = topic;
    publish_param.message.payload = payload;
    publish_param.message_id = 0; // QoS 0, no message id needed
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    mqtt_publish(&client, &publish_param);
}

K_WORK_DEFINE(publish_work, publish_state);

void main(void) {
    struct sockaddr_in broker_addr;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
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

    while (1) {
        k_work_submit(&publish_work);
        k_sleep(K_SECONDS(5));
    }
}
