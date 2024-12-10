
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/sys/printk.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

void mqtt_connect(void) {
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    client_ctx.broker = &broker_addr;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.evt_cb = NULL;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("MQTT connection failed: %d\n", ret);
    } else {
        printk("MQTT connected\n");
    }
}

void mqtt_publish(void) {
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = state_str;
    publish_param.message.payload.len = strlen(state_str);
    publish_param.message_id = 0;

    int ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        printk("MQTT publish failed: %d\n", ret);
    } else {
        printk("MQTT published: %s\n", state_str);
    }
}

void main(void) {
    wifi_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        k_sleep(K_SECONDS(5));
    }
}
