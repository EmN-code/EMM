
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
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
static struct mqtt_utf8 client_id = {
    .utf8 = "zephyr_client",
    .size = 13
};

static void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        LOG_ERR("WiFi connection failed");
    } else {
        LOG_INF("WiFi connected");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
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

static void mqtt_publish(struct mqtt_client *client, const char *payload)
{
    struct mqtt_pub_param param = {
        .message.topic.utf8 = MQTT_TOPIC,
        .message.topic.size = strlen(MQTT_TOPIC),
        .message.payload.data = payload,
        .message.payload.len = strlen(payload),
        .message.qos = MQTT_QOS_0_AT_MOST_ONCE,
        .message.retained = 0,
    };

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("MQTT publish failed %d", ret);
    } else {
        LOG_INF("MQTT published: %s", payload);
    }
}

int main(void)
{
    wifi_connect();

    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker_addr;
    client_ctx.client_id = &client_id;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed %d", ret);
        return -1;
    }

    while (1) {
        mqtt_publish(&client_ctx, "work");
        k_sleep(K_SECONDS(5));
        mqtt_publish(&client_ctx, "not work");
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
