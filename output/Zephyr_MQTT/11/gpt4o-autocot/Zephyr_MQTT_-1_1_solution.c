
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

LOG_MODULE_REGISTER(mqtt_example);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void message_cb(struct mqtt_client *const client,
                       const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("Connected to MQTT broker");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from MQTT broker");
        break;
    default:
        break;
    }
}

static int connect_mqtt(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.evt_cb = message_cb;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    return mqtt_connect(&client_ctx);
}

static void publish_state(void)
{
    static bool state = false;
    const char *state_str = state ? "work" : "not work";

    publish_param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    publish_param.message.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = (uint8_t *)state_str;
    publish_param.message.payload.len = strlen(state_str);
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;
    publish_param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    int ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Published: %s", state_str);
    }

    state = !state;
}

int main(void)
{
    int ret;

    LOG_INF("Starting MQTT example");

    ret = connect_mqtt();
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state();
    }

    return 0;
}
