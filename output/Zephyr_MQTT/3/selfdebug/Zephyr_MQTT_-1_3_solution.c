
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 client_id = {
    .utf8 = "zephyr_client",
    .size = 13
};

static void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PASSWORD,
        .psk_length = strlen(WIFI_PASSWORD),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        LOG_ERR("WiFi connection failed");
    } else {
        LOG_INF("WiFi connected");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
        } else {
            LOG_INF("MQTT connected");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT disconnected");
        break;
    default:
        break;
    }
}

static void mqtt_publish(void)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
        .qos = MQTT_QOS_0_AT_MOST_ONCE
    };

    param.message.topic = &topic;
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    if (mqtt_publish(&client_ctx, &param) != 0) {
        LOG_ERR("MQTT publish failed");
    } else {
        LOG_INF("MQTT published");
    }
}

static void mqtt_connect(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.client_id = &client_id;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client_ctx) != 0) {
        LOG_ERR("MQTT connect failed");
    }
}

void main(void)
{
    wifi_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        k_sleep(K_SECONDS(5));
    }
}
