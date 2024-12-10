
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_transport_tcp_secure transport;

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
                                    uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        struct wifi_status status = {0};

        if (net_mgmt_event_get_params(cb, &status, sizeof(struct wifi_status)) == 0) {
            if (status.status == WIFI_STATUS_CONN_SUCCESS) {
                LOG_INF("WiFi connected");
            } else {
                LOG_ERR("WiFi connection failed: %d", status.status);
            }
        }
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            LOG_INF("Connected to MQTT broker");
        } else {
            LOG_ERR("Failed to connect to MQTT broker: %d", evt->result);
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from MQTT broker");
        break;
    default:
        break;
    }
}

static int connect_to_mqtt_broker(void)
{
    struct mqtt_client_config cfg = {
        .client_id.utf8 = "zephyr_client",
        .client_id.size = strlen("zephyr_client"),
        .keepalive = 60,
        .clean_session = true,
        .user_name = NULL,
        .password = NULL,
        .will_topic = NULL,
        .will_message = NULL,
        .transport.type = MQTT_TRANSPORT_NON_SECURE,
        .transport.tcp.sock = -1,
    };

    client_ctx.broker = &transport;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_client_init(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to initialize MQTT client: %d", ret);
        return ret;
    }

    ret = mqtt_connect(&client_ctx, &cfg);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return ret;
    }

    return 0;
}

static int publish_state(void)
{
    struct mqtt_publish_param param = {
        .message.topic.utf8 = MQTT_TOPIC,
        .message.topic.size = strlen(MQTT_TOPIC),
        .message.payload.data = MQTT_PAYLOAD,
        .message.payload.len = strlen(MQTT_PAYLOAD),
        .message.qos = MQTT_QOS,
        .message.retained = MQTT_RETAIN,
        .dup_flag = 0,
        .message_id = 0,
    };

    int ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
        return ret;
    }

    LOG_INF("Published state: %s", MQTT_PAYLOAD);
    return 0;
}

int main(void)
{
    struct net_mgmt_event_callback wifi_cb;
    struct net_if *iface = net_if_get_default();

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params,
                       sizeof(struct wifi_connect_req_params));
    if (ret) {
        LOG_ERR("Failed to request WiFi connection: %d", ret);
        return ret;
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    ret = connect_to_mqtt_broker();
    if (ret) {
        return ret;
    }

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
