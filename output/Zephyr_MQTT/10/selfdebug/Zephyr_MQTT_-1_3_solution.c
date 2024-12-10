
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        const struct wifi_status *status = (const struct wifi_status *)cb->info;
        if (status->status) {
            LOG_INF("Connection request failed (%d)", status->status);
        } else {
            LOG_INF("Connected to %s", WIFI_SSID);
        }
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        const struct wifi_status *status = (const struct wifi_status *)cb->info;
        if (status->status) {
            LOG_INF("Disconnection request failed (%d)", status->status);
        } else {
            LOG_INF("Disconnected from %s", WIFI_SSID);
        }
    }
}

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                LOG_ERR("MQTT connect failed %d", evt->result);
            } else {
                LOG_INF("MQTT connected!");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT disconnected");
            break;
        default:
            break;
    }
}

int main(void)
{
    struct net_mgmt_event_callback wifi_mgmt_cb;
    struct sockaddr_in broker;
    int ret;

    net_mgmt_init_event_callback(&wifi_mgmt_cb, wifi_mgmt_event_handler, NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_mgmt_cb);

    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(struct wifi_connect_req_params));
    if (ret) {
        LOG_ERR("WiFi connection request failed (%d)", ret);
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    memset(&client_ctx, 0, sizeof(client_ctx));
    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = (uint8_t *)"zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    mqtt_client_init(&client_ctx);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed %d", ret);
        return -1;
    }

    while (1) {
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        publish_param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        publish_param.message.topic.size = strlen(MQTT_TOPIC);
        publish_param.message.payload.data = (uint8_t *)state;
        publish_param.message.payload.len = strlen(state);
        publish_param.message_id = 0;
        publish_param.dup_flag = 0;
        publish_param.retain_flag = 0;

        ret = mqtt_publish(&client_ctx, &publish_param);
        if (ret) {
            LOG_ERR("MQTT publish failed %d", ret);
        } else {
            LOG_INF("Published: %s", state);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
