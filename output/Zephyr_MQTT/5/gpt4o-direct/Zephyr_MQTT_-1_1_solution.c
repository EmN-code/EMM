
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
static struct mqtt_tx_buf tx;
static struct mqtt_rx_buf rx;

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

void connect_to_broker(void)
{
    int ret;
    struct sockaddr_in broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.password = (uint8_t *)MQTT_PASSWORD;
    client_ctx.password_len = (MQTT_PASSWORD != NULL) ? strlen(MQTT_PASSWORD) : 0;
    client_ctx.user_name = (uint8_t *)MQTT_USERNAME;
    client_ctx.user_name_len = (MQTT_USERNAME != NULL) ? strlen(MQTT_USERNAME) : 0;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed %d", ret);
    }
}

void publish_state(const char *state)
{
    int ret;
    struct mqtt_pub_msg msg;

    msg.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    msg.topic.size = strlen(MQTT_TOPIC);
    msg.payload.utf8 = (uint8_t *)state;
    msg.payload.size = strlen(state);
    msg.qos = MQTT_QOS_0_AT_MOST_ONCE;
    msg.retain = 0;

    ret = mqtt_publish(&client_ctx, &msg);
    if (ret) {
        LOG_ERR("MQTT publish failed %d", ret);
    } else {
        LOG_INF("Published: %s", state);
    }
}

void main(void)
{
    connect_to_broker();

    while (1) {
        publish_state("work");
        k_sleep(K_SECONDS(5));
        publish_state("not work");
        k_sleep(K_SECONDS(5));
    }
}
