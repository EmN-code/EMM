
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
#include <zephyr/logging/log.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params;

    params.ssid = WIFI_SSID;
    params.ssid_length = strlen(WIFI_SSID);
    params.psk = WIFI_PSK;
    params.psk_length = strlen(WIFI_PSK);
    params.channel = WIFI_CHANNEL_ANY;
    params.security = WIFI_SECURITY_TYPE_PSK;

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        LOG_ERR("WiFi connection failed");
    } else {
        LOG_INF("WiFi connected");
    }
}

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            LOG_INF("MQTT client connected!");
            publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
            publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
            publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
            publish_param.message.payload.data = MQTT_PAYLOAD;
            publish_param.message.payload.len = strlen(MQTT_PAYLOAD);
            publish_param.message_id = 1U;
            publish_param.dup_flag = 0U;
            publish_param.retain_flag = 0U;

            mqtt_publish(client, &publish_param);
        } else {
            LOG_ERR("MQTT connect failed %d", evt->result);
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected %d", evt->result);
        break;
    default:
        break;
    }
}

void mqtt_connect(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    memset(&broker, 0, sizeof(broker));
    broker4->sin_family = AF_INET;
    broker4->sin_addr.s_addr = htonl(INADDR_ANY);
    broker4->sin_port = htons(MQTT_BROKER_PORT);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_mqtt_client";
    client_ctx.client_id.size = strlen("zephyr_mqtt_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.clean_session = 1;
    client_ctx.keepalive = 60U;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client_ctx);
}

void main(void)
{
    wifi_connect();
    mqtt_connect();

    while (1) {
        k_sleep(K_SECONDS(5));
        mqtt_publish(&client_ctx, &publish_param);
    }
}
