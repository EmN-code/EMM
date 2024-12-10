
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/drivers/flash.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_connect_param connect_param;

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

void publish_state(struct mqtt_client *client)
{
    static bool state = true;
    const char *payload = state ? "work" : "not work";
    state = !state;

    struct mqtt_publish_param param;
    struct mqtt_publish_message msg;

    msg.topic.utf8 = MQTT_TOPIC;
    msg.topic.size = strlen(MQTT_TOPIC);
    msg.payload.data = payload;
    msg.payload.len = strlen(payload);
    msg.qos = MQTT_QOS_0_AT_MOST_ONCE;

    param.message = msg;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&connect_param, 0, sizeof(connect_param));

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "esp32_client";
    client_ctx.client_id.size = strlen("esp32_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.clean_session = 1;
    client_ctx.keepalive = 60;
    client_ctx.will_topic = NULL;
    client_ctx.will_msg = NULL;
    client_ctx.will_qos = 0;
    client_ctx.will_retain = 0;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client_ctx);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }

    return 0;
}
