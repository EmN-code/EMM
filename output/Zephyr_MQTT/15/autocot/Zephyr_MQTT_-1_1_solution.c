
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
}

static void mqtt_connect(void) {
    int rc;
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
    client_ctx.socket = -1;

    rc = mqtt_connect(&client_ctx);
    if (rc) {
        printk("MQTT connect failed: %d\n", rc);
        return;
    }

    printk("MQTT connected\n");
}

static void mqtt_publish(const char *state) {
    int rc;
    struct mqtt_publish_param param;

    param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.utf8 = (uint8_t *)state;
    param.message.payload.size = strlen(state);
    param.message_id = 1U;
    param.dup_flag = 0U;
    param.retain_flag = 0U;
    param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    rc = mqtt_publish(&client_ctx, &param);
    if (rc) {
        printk("MQTT publish failed: %d\n", rc);
    } else {
        printk("MQTT published: %s\n", state);
    }
}

int main(void) {
    wifi_connect();
    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    mqtt_connect();

    while (1) {
        mqtt_publish("work");
        k_sleep(K_SECONDS(5));
        mqtt_publish("not work");
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
