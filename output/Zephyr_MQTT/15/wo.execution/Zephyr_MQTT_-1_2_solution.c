
#include <zephyr/sys/printk.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/mqtt_sn.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_msg;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
        .channel = WIFI_CHANNEL_ANY,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

void mqtt_init(void) {
    memset(&client, 0, sizeof(client));
    client.broker = MQTT_BROKER_ADDR;
    client.broker_port = MQTT_BROKER_PORT;
    client.client_id = "esp32_client";
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = 1;

    struct mqtt_connect_param connect_params = {
        .keepalive = 60,
        .clean_session = 1,
        .client_id = client.client_id,
        .client_id_len = strlen(client.client_id),
        .user_name = NULL,
        .user_name_len = 0,
        .password = NULL,
        .password_len = 0,
    };

    if (mqtt_connect(&client, &connect_params) != 0) {
        printk("MQTT connection failed\n");
    } else {
        printk("MQTT connected\n");
    }
}

void mqtt_publish_state(void) {
    publish_msg.topic.utf8 = MQTT_TOPIC;
    publish_msg.topic.size = strlen(MQTT_TOPIC);
    publish_msg.payload.data = MQTT_PAYLOAD;
    publish_msg.payload.len = strlen(MQTT_PAYLOAD);
    publish_msg.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_msg.retain = 0;

    publish_param.message = publish_msg;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    if (mqtt_publish(&client, &publish_param) != 0) {
        printk("MQTT publish failed\n");
    } else {
        printk("MQTT published\n");
    }
}

int main(void) {
    wifi_connect();
    mqtt_init();

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
