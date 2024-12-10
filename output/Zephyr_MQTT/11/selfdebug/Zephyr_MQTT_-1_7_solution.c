
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_CLIENT_ID "zephyr_mqtt_client"

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 client_id = {
    .utf8 = MQTT_CLIENT_ID,
    .size = sizeof(MQTT_CLIENT_ID) - 1
};

static void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = sizeof(WIFI_SSID) - 1,
        .psk = WIFI_PSK,
        .psk_length = sizeof(WIFI_PSK) - 1,
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

static void mqtt_publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param = {
        .message = {
            .topic = {
                .topic = MQTT_TOPIC,
                .topic_len = sizeof(MQTT_TOPIC) - 1
            },
            .payload.data = MQTT_PAYLOAD,
            .payload.len = sizeof(MQTT_PAYLOAD) - 1,
            .qos = MQTT_QOS_1_AT_LEAST_ONCE,
            .retain = 0
        },
        .message_id = 1234,
        .dup_flag = 0,
        .retain_flag = 0
    };

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Message published");
    }
}

static void mqtt_connect(void)
{
    struct sockaddr_in broker = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.client_id = &client_id;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
    }
}

int main(void)
{
    wifi_connect();
    mqtt_connect();

    while (1) {
        if (client_ctx.internal.state == MQTT_STATE_CONNECTED) {
            mqtt_publish(&client_ctx);
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
