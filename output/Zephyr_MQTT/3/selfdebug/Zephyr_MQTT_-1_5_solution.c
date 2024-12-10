
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const client,
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

void connect_to_mqtt_broker(void)
{
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.clean_session = MQTT_CLEAN_SESSION;
    client_ctx.keepalive = MQTT_KEEPALIVE;
    client_ctx.user_name = NULL;
    client_ctx.password = NULL;
    client_ctx.will_topic = NULL;
    client_ctx.will_message = NULL;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed %d", ret);
    }
}

void publish_state(void)
{
    int ret;
    struct mqtt_publish_param param;

    param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = MQTT_PAYLOAD_LEN;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        LOG_ERR("MQTT publish failed %d", ret);
    } else {
        LOG_INF("State published");
    }
}

void main(void)
{
    connect_to_mqtt_broker();

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state();
    }
}
