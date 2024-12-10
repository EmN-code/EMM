
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/init.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/types.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct sockaddr_in broker;

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

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic;
    static uint16_t message_id = 0;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    param.message.topic = topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = message_id++;
    param.dup_flag = 0;
    param.retain_flag = 0;

    if (mqtt_publish(client, &param) != 0) {
        printk("Failed to publish message\n");
    } else {
        printk("Message published\n");
    }
}

int main(void)
{
    int sock;
    struct mqtt_connect_param conn_param;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    mqtt_client_init(&client);
    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.evt_cb = mqtt_evt_handler;

    conn_param.client_id = "zephyr_client";
    conn_param.keepalive = 60;
    conn_param.clean_session = 1;
    conn_param.user_name = NULL;
    conn_param.password = NULL;
    conn_param.will_topic = NULL;
    conn_param.will_msg = NULL;
    conn_param.will_qos = 0;
    conn_param.will_retain = 0;
    conn_param.will_msg_len = 0;

    if (mqtt_connect(&client, &conn_param) != 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
