#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <time.h>
#include <sys/time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static char client_id[] = "zephyr_publisher";
static char topic[] = "device/state";
static char message[] = "work";

void main(void) {
    struct mqtt_client client;
    struct sockaddr_in broker;
    struct mqtt_publish_param publish_param;
    int ret;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.client_id.utf8 = (uint8_t *)client_id;
    client.client_id.size = strlen(client_id);

    ret = mqtt_connect(&client, (struct sockaddr *)&broker, MQTT_TIMEOUT);
    if (ret != 0) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    while (1) {
        publish_param.message.topic.topic.utf8 = (uint8_t *)topic;
        publish_param.message.topic.topic.size = strlen(topic);
        publish_param.message.payload.data = message;
        publish_param.message.payload.len = strlen(message);
        publish_param.message_id = sys_rand32_get();
        publish_param.dup_flag = 0;
        publish_param.retain_flag = 0;

        ret = mqtt_publish(&client, &publish_param);
        if (ret != 0) {
            printk("Failed to publish message: %d\n", ret);
        }

        k_sleep(K_SECONDS(5));
    }
}