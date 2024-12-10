
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/sem.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/wifi.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <time.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/device.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;

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
        default:
            break;
    }
}

void publish_state(struct k_work *work)
{
    int ret;

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    ret = mqtt_publish(&client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published\n");
    }

    k_work_reschedule(&work, K_SECONDS(5));
}

K_WORK_DEFINE(publish_work, publish_state);

int main(void)
{
    struct sockaddr_in broker_addr;
    int ret;

    memset(&client, 0, sizeof(client));
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_MESSAGE;
    publish_message.payload.len = strlen(MQTT_MESSAGE);
    publish_message.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    k_work_submit(&publish_work);

    return 0;
}
