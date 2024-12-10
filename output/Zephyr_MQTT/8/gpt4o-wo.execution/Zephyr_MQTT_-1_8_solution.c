
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/random/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct sockaddr_storage broker;

void publish_state(struct mqtt_client *client) {
    static bool state = false;
    const char *message = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = message;
    publish_param.message.payload.len = strlen(message);
    publish_param.message.topic.qos = MQTT_QOS;
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    mqtt_publish(client, &publish_param);
}

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                printk("Failed to connect to MQTT broker: %d\n", evt->result);
                return;
            }
            printk("Connected to MQTT broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVT_PUBACK:
            printk("Message published\n");
            break;
        default:
            break;
    }
}

int main(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    int rc;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;

    rc = mqtt_connect(&client_ctx);
    if (rc) {
        printk("MQTT connect failed: %d\n", rc);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }

    return 0;
}
