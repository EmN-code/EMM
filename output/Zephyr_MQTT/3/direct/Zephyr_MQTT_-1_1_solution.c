
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define STACK_SIZE 1024
#define PRIORITY 7

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 topic;
static struct mqtt_utf8 client_id;
static struct sockaddr_in broker;

void publish_state(struct mqtt_client *client) {
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    int ret;
    struct mqtt_publish_param param;

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    param.message.topic = topic;
    param.message.payload.data = state_str;
    param.message.payload.len = strlen(state_str);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Published: %s", state_str);
    }
}

void mqtt_evt_handler(struct mqtt_client *const client,
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

void main(void) {
    int ret;
    struct mqtt_client_config config;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&config, 0, sizeof(config));

    client_id.utf8 = MQTT_CLIENT_ID;
    client_id.size = strlen(MQTT_CLIENT_ID);

    config.client_id = client_id;
    config.user_name = MQTT_USERNAME;
    config.password = MQTT_PASSWORD;
    config.keepalive = 60;

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_cfg = &config;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
