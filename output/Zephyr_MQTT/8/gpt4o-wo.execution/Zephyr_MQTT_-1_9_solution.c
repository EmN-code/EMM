
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/sys_heap.h>
#include <zephyr/sys/kheap.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/sem.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/net/coap.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/device.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("Connected to MQTT broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

void publish_state(struct k_work *work)
{
    static int message_id = 0;
    struct mqtt_publish_param param;
    struct mqtt_publish_message msg;

    msg.topic.utf8 = MQTT_TOPIC;
    msg.topic.size = strlen(MQTT_TOPIC);
    msg.payload.data = MQTT_PAYLOAD;
    msg.payload.len = strlen(MQTT_PAYLOAD);
    msg.qos = MQTT_QOS;
    msg.retain = MQTT_RETAIN;

    param.message = msg;
    param.message_id = message_id++;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(&client, &param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published\n");
    }
}

K_WORK_DEFINE(publish_work, publish_state);

void main(void)
{
    int ret;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);
    client.broker = (struct sockaddr *)broker_addr;
    client.broker_len = sizeof(broker);
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    while (1) {
        k_work_submit(&publish_work);
        k_sleep(K_SECONDS(5));
    }
}
