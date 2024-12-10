
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/wifi.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <time.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_QOS 1

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_topic topic;
struct mqtt_publish_message message;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected\n");
            break;
        case MQTT_EVT_PUBACK:
            printk("MQTT PUBACK received\n");
            break;
        default:
            break;
    }
}

void publish_state(struct k_work *work)
{
    static int state = 0;
    const char *payload = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic = topic;
    publish_param.message.payload.data = payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    mqtt_publish(&client, &publish_param);
}

K_WORK_DEFINE(publish_work, publish_state);

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&client, 0, sizeof(client));
    memset(&topic, 0, sizeof(topic));
    memset(&message, 0, sizeof(message));

    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.evt_cb = mqtt_evt_handler;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);

    message.qos = MQTT_QOS;

    mqtt_client_init(&client);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("MQTT connect failed: %d\n", ret);
        return ret;
    }

    while (1) {
        k_work_submit(&publish_work);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
