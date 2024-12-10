
#include <zephyr/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            printk("Connected to MQTT broker\n");
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

void publish_state(struct mqtt_client *client)
{
    static bool state = false;
    char payload[16];
    struct mqtt_publish_param param;

    snprintf(payload, sizeof(payload), "%s", state ? "work" : "not work");
    state = !state;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", payload);
    }
}

void main(void)
{
    struct sockaddr_in broker;
    struct mqtt_connect_param connect_params;

    memset(&client_ctx, 0, sizeof(client_ctx));

    client_ctx.client_id.utf8 = MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);
    client_ctx.keepalive = MQTT_KEEPALIVE;
    client_ctx.clean_session = MQTT_CLEAN_SESSION;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client_ctx.broker = &broker;

    connect_params.client_id = client_ctx.client_id;
    connect_params.user_name = NULL;
    connect_params.password = NULL;
    connect_params.keepalive = client_ctx.keepalive;
    connect_params.clean_session = client_ctx.clean_session;
    connect_params.will_topic = NULL;
    connect_params.will_message = NULL;
    connect_params.will_qos = 0;
    connect_params.will_retain = 0;

    int ret = mqtt_connect(&client_ctx, &connect_params);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
