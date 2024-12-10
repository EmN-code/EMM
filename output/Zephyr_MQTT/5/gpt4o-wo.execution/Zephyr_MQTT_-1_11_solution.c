#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/stats/stats.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/buf.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/websocket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/printk.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param param;
static struct k_timer timer;

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected!\n");
        break;
    default:
        break;
    }
}

void mqtt_publish_work_status(void)
{
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "work/status";
    param.message.topic.topic.size = strlen("work/status");
    param.message.payload.data = "work";
    param.message.payload.len = strlen("work");

    if (mqtt_publish(&client, &param)) {
        printk("Error publishing\n");
    } else {
        printk("Published: work\n");
    }
}

void timer_handler(struct k_timer *dummy)
{
    mqtt_publish_work_status();
}

int main(void)
{
    printk("MQTT sample started\n");

    struct sockaddr_in *broker = (struct sockaddr_in *)&client.broker;
    broker->sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &broker->sin_addr);
    broker->sin_port = htons(SERVER_PORT);

    mqtt_client_init(&client);
    client.broker = broker;
    client.evt_cb = mqtt_evt_handler;

    if (mqtt_connect(&client)) {
        printk("Error connecting to the broker\n");
        return -1;
    }

    k_timer_init(&timer, timer_handler, NULL);
    k_timer_start(&timer, K_NO_WAIT, PUBLISH_INTERVAL);

    while (true) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_MSEC(100));
    }

    return 0;
}