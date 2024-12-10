
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
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
static struct mqtt_publish_param publish_param;

void wifi_connect(void) {
    const struct device *wifi_dev = device_get_binding("esp32_wifi");
    if (!wifi_dev) {
        LOG_ERR("WiFi device not found");
        return;
    }

    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = WIFI_CHANNEL_ANY,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (wifi_connect(wifi_dev, &wifi_params) < 0) {
        LOG_ERR("Failed to connect to WiFi");
    } else {
        LOG_INF("Connected to WiFi");
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

void mqtt_publish_state(struct mqtt_client *client, const char *state) {
    struct mqtt_publish_param param = {
        .message = {
            .topic = {
                .topic.utf8 = MQTT_TOPIC,
                .topic.size = strlen(MQTT_TOPIC),
            },
            .payload.data = state,
            .payload.len = strlen(state),
        },
        .dup_flag = 0,
        .retain_flag = 0,
        .message_id = 0,
    };

    int ret = mqtt_publish(client, &param);
    if (ret < 0) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Published state: %s", state);
    }
}

int main(void) {
    wifi_connect();

    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker_addr;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return -1;
    }

    while (1) {
        mqtt_publish_state(&client_ctx, "work");
        k_sleep(K_SECONDS(5));
        mqtt_publish_state(&client_ctx, "not work");
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
