
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/logging/log.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PASSWORD,
        .psk_length = strlen(WIFI_PASSWORD),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params));
    if (ret < 0) {
        LOG_ERR("WiFi connection failed: %d", ret);
    } else {
        LOG_INF("WiFi connected");
    }
}

void mqtt_connect(void) {
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    memset(&client_ctx, 0, sizeof(client_ctx));
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
        LOG_ERR("MQTT connection failed: %d", ret);
    } else {
        LOG_INF("MQTT connected");
    }
}

void publish_state(void) {
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    struct mqtt_publish_param param = {
        .message.topic.topic.utf8 = MQTT_TOPIC,
        .message.topic.topic.size = strlen(MQTT_TOPIC),
        .message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE,
        .message.payload.data = state_str,
        .message.payload.len = strlen(state_str),
        .dup_flag = 0,
        .retain_flag = 0,
    };

    int ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        LOG_ERR("MQTT publish failed: %d", ret);
    } else {
        LOG_INF("MQTT state published: %s", state_str);
    }
}

void main(void) {
    wifi_connect();
    mqtt_connect();

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}
