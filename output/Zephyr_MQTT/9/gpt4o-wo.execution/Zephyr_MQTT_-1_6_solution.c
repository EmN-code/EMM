
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/offloaded_netdev.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/init.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/random/random.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <time.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/types.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define MQTT_QOS 0

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;

void mqtt_client_init(struct mqtt_client *client) {
    client->broker = (struct sockaddr *)&client->broker_addr;
    client->broker_len = sizeof(client->broker_addr);
    client->transport.type = MQTT_TRANSPORT_NON_SECURE;
    client->client_id.utf8 = "zephyr_client";
    client->client_id.size = strlen(client->client_id.utf8);
}

int mqtt_connect(struct mqtt_client *client) {
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&client->broker_addr;
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr->sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        return -1;
    }

    if (connect(sock, (struct sockaddr *)broker_addr, sizeof(*broker_addr)) < 0) {
        close(sock);
        return -1;
    }

    client->transport.tcp.sock = sock;
    return 0;
}

int mqtt_publish(struct mqtt_client *client, const struct mqtt_publish_param *param) {
    // Implement MQTT publish logic here
    return 0;
}

int main(void) {
    mqtt_client_init(&client);
    if (mqtt_connect(&client) < 0) {
        printk("Failed to connect to MQTT broker\n");
        return -1;
    }

    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD_WORK;
    publish_message.payload.len = strlen(MQTT_PAYLOAD_WORK);
    publish_message.qos = MQTT_QOS;

    publish_param.message = publish_message;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    while (1) {
        mqtt_publish(&client, &publish_param);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
