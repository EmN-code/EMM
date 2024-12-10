
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/logging/log.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

LOG_MODULE_REGISTER(main);

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
    if (ret) {
        LOG_ERR("WiFi connection failed: %d", ret);
    } else {
        LOG_INF("WiFi connected");
    }
}

void mqtt_connect(void) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr->sin_addr);

    int ret = connect(sock, (struct sockaddr *)broker_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        close(sock);
        return;
    }

    mqtt_client_init(&client_ctx);
    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(struct sockaddr_in);
    client_ctx.client_id.utf8 = (uint8_t *)"esp32_client";
    client_ctx.client_id.size = strlen("esp32_client");
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = sock;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed: %d", ret);
        close(sock);
        return;
    }

    LOG_INF("MQTT connected");
}

void publish_state(const char *state) {
    publish_param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    publish_param.message.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.utf8 = (uint8_t *)state;
    publish_param.message.payload.size = strlen(state);
    publish_param.message_id = 1;
    publish_param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    int ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        LOG_ERR("MQTT publish failed: %d", ret);
    } else {
        LOG_INF("Published state: %s", state);
    }
}

void main(void) {
    wifi_connect();
    mqtt_connect();

    while (1) {
        publish_state("work");
        k_sleep(K_SECONDS(5));
        publish_state("not work");
        k_sleep(K_SECONDS(5));
    }
}
