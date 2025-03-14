
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/byteorder.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 topic;
static struct mqtt_client_config config;
static struct sockaddr_storage broker;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

void mqtt_connect(void) {
    int ret;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    config.client_id = "zephyr_client";
    config.user_name = NULL;
    config.password = NULL;
    config.keepalive = 60;
    config.clean_session = true;
    config.timeout = 4000;

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker;
    client_ctx.client_cfg = &config;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("MQTT connection failed: %d\n", ret);
    } else {
        printk("MQTT connected\n");
    }
}

void mqtt_publish(const char *state) {
    int ret;
    struct mqtt_publish_param param;

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    param.message.topic = &topic;
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = 1U;
    param.dup_flag = 0U;
    param.retain_flag = 0U;
    param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        printk("MQTT publish failed: %d\n", ret);
    } else {
        printk("MQTT published: %s\n", state);
    }
}

void main(void) {
    wifi_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish("work");
        k_sleep(K_SECONDS(5));
        mqtt_publish("not work");
        k_sleep(K_SECONDS(5));
    }
}
