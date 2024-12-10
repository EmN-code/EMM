
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;
static struct mqtt_binstr payload;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
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
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.client_id.utf8 = (uint8_t *)"zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    if (mqtt_connect(&client_ctx) != 0) {
        printk("MQTT connect failed\n");
    } else {
        printk("MQTT connected\n");
    }
}

void mqtt_publish(void) {
    topic.utf8 = (uint8_t *)MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);
    payload.utf8 = (uint8_t *)MQTT_PAYLOAD;
    payload.size = MQTT_PAYLOAD_LEN;

    publish_param.message.topic = topic;
    publish_param.message.payload = payload;
    publish_param.message.payload_len = MQTT_PAYLOAD_LEN;
    publish_param.message.qos = MQTT_QOS;
    publish_param.message.retained = MQTT_RETAIN;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;
    publish_param.message_id = 0;

    if (mqtt_publish(&client_ctx, &publish_param) != 0) {
        printk("MQTT publish failed\n");
    } else {
        printk("MQTT published\n");
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
