
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(mqtt_example);

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
    if (ret) {
        LOG_ERR("WiFi connect failed: %d", ret);
    } else {
        LOG_INF("WiFi connected");
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

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.client_id.utf8 = (uint8_t *)"zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.evt_cb = NULL;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed: %d", ret);
    } else {
        LOG_INF("MQTT connected");
    }
}

static void publish_state(const char *state)
{
    publish_param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    publish_param.message.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.utf8 = (uint8_t *)state;
    publish_param.message.payload.size = strlen(state);
    publish_param.message_id = 1U;
    publish_param.dup_flag = 0U;
    publish_param.retain_flag = 0U;
    publish_param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    int ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        LOG_ERR("MQTT publish failed: %d", ret);
    } else {
        LOG_INF("MQTT published: %s", state);
    }
}

int main(void)
{
    wifi_connect();
    mqtt_connect();

    while (1) {
        publish_state("work");
        k_sleep(K_SECONDS(5));
        publish_state("not work");
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
