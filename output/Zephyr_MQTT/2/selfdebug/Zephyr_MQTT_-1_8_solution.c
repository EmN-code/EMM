
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
static struct mqtt_transport transport;

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

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        LOG_ERR("WiFi connection failed");
    } else {
        LOG_INF("WiFi connected");
    }
}

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
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

void publish_state(struct mqtt_client *client) {
    static bool state = false;
    char payload[20];
    snprintf(payload, sizeof(payload), "%s", state ? "work" : "not work");
    state = !state;

    struct mqtt_publish_param param = {
        .message = {
            .topic = {
                .topic.utf8 = MQTT_TOPIC,
                .topic.size = strlen(MQTT_TOPIC),
            },
            .payload.data = payload,
            .payload.len = strlen(payload),
            .qos = MQTT_QOS_0_AT_MOST_ONCE,
            .retain = 0,
        },
        .dup_flag = 0,
        .retain_flag = 0,
    };

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Published: %s", payload);
    }
}

void main(void) {
    wifi_connect();

    transport.type = MQTT_TRANSPORT_NON_SECURE;
    transport.tcp.sock = -1;

    mqtt_client_init(&client_ctx);
    client_ctx.transport = &transport;
    client_ctx.broker = MQTT_BROKER_IP;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
