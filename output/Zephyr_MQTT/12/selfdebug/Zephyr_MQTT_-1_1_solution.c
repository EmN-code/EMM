
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/sys/printk.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
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

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
    if (ret < 0) {
        printk("WiFi connection failed: %d\n", ret);
    } else {
        printk("WiFi connected\n");
    }
}

void mqtt_connect(void) {
    int sock;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create MQTT socket: %d\n", sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)broker4, sizeof(*broker4)) < 0) {
        printk("Failed to connect to MQTT broker: %d\n", errno);
        close(sock);
        return;
    }

    mqtt_client_init(&client_ctx);
    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = sock;

    if (mqtt_connect(&client_ctx) != 0) {
        printk("MQTT connect failed\n");
        close(sock);
        return;
    }

    printk("MQTT connected\n");
}

void mqtt_publish(const char *state) {
    int ret;

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = state;
    publish_param.message.payload.len = strlen(state);
    publish_param.message_id = 1U;

    ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret != 0) {
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
