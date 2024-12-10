
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <net/mqtt.h>
#include <net/net_mgmt.h>
#include <net/net_event.h>
#include <net/net_if.h>
#include <net/wifi_mgmt.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params))) {
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
                LOG_ERR("Failed to connect to MQTT broker, error: %d", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("Disconnected from MQTT broker");
            break;
        default:
            break;
    }
}

void mqtt_publish_state(struct mqtt_client *client) {
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = state_str;
    publish_param.message.payload.len = strlen(state_str);
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    if (mqtt_publish(client, &publish_param) != 0) {
        LOG_ERR("Failed to publish message");
    } else {
        LOG_INF("Published: %s", state_str);
    }
}

void main(void) {
    wifi_connect();

    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker_addr;
    client_ctx.client_id.utf8 = MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.user_name = NULL;
    client_ctx.password = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client_ctx) != 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        return;
    }

    while (1) {
        mqtt_publish_state(&client_ctx);
        k_sleep(K_SECONDS(5));
    }
}
