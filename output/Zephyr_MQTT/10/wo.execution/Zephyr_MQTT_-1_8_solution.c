
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/random/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <sys/time.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_tx_buf tx;
static struct mqtt_rx_buf rx;

void mqtt_evt_handler(struct mqtt_client *const client,
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

int main(void)
{
    struct sockaddr_in broker;
    struct mqtt_connect_param conn_param;
    struct mqtt_publish_param pub_param;
    struct mqtt_publish_message pub_msg;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&broker, 0, sizeof(broker));
    memset(&conn_param, 0, sizeof(conn_param));
    memset(&pub_param, 0, sizeof(pub_param));
    memset(&pub_msg, 0, sizeof(pub_msg));

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    broker.sin_family = AF_INET;
    broker.sin_addr.s_addr = htonl(INADDR_ANY);
    broker.sin_port = htons(MQTT_BROKER_PORT);

    conn_param.keepalive = 60U;
    conn_param.clean_session = true;
    conn_param.client_id = &client_ctx.client_id;
    conn_param.user_name = client_ctx.user_name;
    conn_param.password = client_ctx.password;
    conn_param.will_topic = NULL;
    conn_param.will_msg = NULL;
    conn_param.will_qos = 0U;
    conn_param.will_retain = 0U;

    pub_msg.topic.utf8 = MQTT_TOPIC;
    pub_msg.topic.size = strlen(MQTT_TOPIC);
    pub_msg.payload.data = MQTT_PAYLOAD;
    pub_msg.payload.len = strlen(MQTT_PAYLOAD);
    pub_msg.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    pub_msg.retain = 0U;

    pub_param.message = pub_msg;
    pub_param.message_id = 1U;
    pub_param.dup_flag = 0U;
    pub_param.retain_flag = 0U;

    mqtt_client_init(&client_ctx);
    mqtt_connect(&client_ctx, &conn_param);

    while (1) {
        mqtt_publish(&client_ctx, &pub_param);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
