
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
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
#define MQTT_TOPIC "device/state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        struct wifi_status status = {0};
        net_mgmt_event_get_wifi_status(cb, &status);
        if (status.status) {
            LOG_ERR("Connection request failed (%d)", status.status);
        } else {
            LOG_INF("Connected to WiFi");
        }
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
        } else {
            LOG_INF("MQTT connected");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT disconnected");
        break;
    default:
        break;
    }
}

static void publish_state(struct mqtt_client *client, const char *state)
{
    int ret;
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("MQTT publish failed %d", ret);
    } else {
        LOG_INF("Published state: %s", state);
    }
}

int main(void)
{
    struct net_mgmt_event_callback wifi_cb;
    struct sockaddr_in broker;
    int ret;

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler, NET_EVENT_WIFI_CONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, net_if_get_default(), &wifi_params, sizeof(wifi_params));
    if (ret) {
        LOG_ERR("WiFi connection request failed");
        return -1;
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed %d", ret);
        return -1;
    }

    while (1) {
        publish_state(&client_ctx, "work");
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx, "not work");
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
