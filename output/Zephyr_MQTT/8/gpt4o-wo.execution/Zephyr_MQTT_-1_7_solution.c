
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
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
#include <zephyr/sys/sys_heap.h>
#include <zephyr/sys/kheap.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/sem.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_tx_buf tx_buf;
static struct mqtt_rx_buf rx_buf;

static K_WORK_DEFINE(publish_work, publish_state);

void publish_state(struct k_work *work)
{
    struct mqtt_publish_param param;
    struct mqtt_publish_message msg;
    static uint16_t message_id = 0;
    static bool state = false;

    const char *state_str = state ? "work" : "not work";
    state = !state;

    msg.topic.utf8 = MQTT_TOPIC;
    msg.topic.size = strlen(MQTT_TOPIC);
    msg.payload.data = state_str;
    msg.payload.len = strlen(state_str);
    msg.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    msg.retain = 0;

    param.message = msg;
    param.message_id = message_id++;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", state_str);
    }

    k_work_reschedule(&publish_work, K_SECONDS(5));
}

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            printk("Connected to MQTT broker\n");
            k_work_submit(&publish_work);
        } else {
            printk("Failed to connect to MQTT broker: %d\n", evt->result);
        }
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    default:
        break;
    }
}

int main(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_addr.s_addr = htonl(0x2F666701); // 47.102.103.1
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);

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

    tx_buf.buf = tx_buffer;
    tx_buf.len = sizeof(tx_buffer);
    rx_buf.buf = rx_buffer;
    rx_buf.len = sizeof(rx_buffer);

    client_ctx.tx_buf = &tx_buf;
    client_ctx.rx_buf = &rx_buf;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return -1;
    }

    k_work_init(&publish_work, publish_state);

    return 0;
}
