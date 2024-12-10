
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
static struct mqtt_client_config client_cfg;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_client *client)
{
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = state_str;
    param.message.payload.len = strlen(state_str);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Published: %s", state_str);
    }
}

void main(void)
{
    int ret;
    struct sockaddr_in broker;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&client_cfg, 0, sizeof(client_cfg));

    client_cfg.client_id.utf8 = MQTT_CLIENT_ID;
    client_cfg.client_id.size = strlen(MQTT_CLIENT_ID);
    client_cfg.user_name.utf8 = MQTT_USERNAME;
    client_cfg.password.utf8 = MQTT_PASSWORD;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client_cfg.broker = &broker;
    client_cfg.evt_cb = mqtt_evt_handler;

    mqtt_client_init(&client_ctx);
    ret = mqtt_connect(&client_ctx, &client_cfg);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
