
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <net/mqtt.h>
#include <net/net_context.h>
#include <net/net_core.h>
#include <net/net_if.h>
#include <net/net_mgmt.h>
#include <net/wifi_mgmt.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params;

    params.ssid = WIFI_SSID;
    params.ssid_length = strlen(WIFI_SSID);
    params.psk = WIFI_PASSWORD;
    params.psk_length = strlen(WIFI_PASSWORD);
    params.channel = WIFI_CHANNEL_ANY;
    params.security = WIFI_SECURITY_TYPE_PSK;

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        LOG_ERR("WiFi connection failed");
    } else {
        LOG_INF("WiFi connected");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected");
            break;
        default:
            break;
    }
}

static void mqtt_publish(const char *message) {
    int ret;
    struct mqtt_client_config cfg;

    memset(&cfg, 0, sizeof(cfg));
    cfg.client_id = MQTT_CLIENT_ID;
    cfg.broker = MQTT_BROKER_IP;
    cfg.port = MQTT_BROKER_PORT;

    mqtt_client_init(&client_ctx);
    client_ctx.config = &cfg;
    client_ctx.evt_cb = mqtt_evt_handler;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed: %d", ret);
        return;
    }

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = message;
    publish_param.message.payload.len = strlen(message);
    publish_param.message_id = 1U;

    ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        LOG_ERR("MQTT publish failed: %d", ret);
    } else {
        LOG_INF("MQTT message published: %s", message);
    }

    mqtt_disconnect(&client_ctx);
}

void main(void) {
    wifi_connect();

    while (1) {
        mqtt_publish("work");
        k_sleep(K_SECONDS(5));
        mqtt_publish("not work");
        k_sleep(K_SECONDS(5));
    }
}
