
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>
#include <stdio.h>
#include <string.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(mqtt_client);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("Connection error: %d", evt->result);
        } else {
            LOG_INF("Connected to broker");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from broker");
        break;
    default:
        break;
    }
}

void publish_message(struct mqtt_client *client)
{
    static uint16_t message_id = 0;
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = ++message_id;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Message published");
    }
}

void main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&client_ctx, 0, sizeof(client_ctx));
    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.clean_session = 1;
    client_ctx.keepalive = 60;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_message(&client_ctx);
    }
}
