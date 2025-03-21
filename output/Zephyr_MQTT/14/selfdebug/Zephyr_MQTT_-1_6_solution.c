
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

LOG_MODULE_REGISTER(mqtt_client);

struct mqtt_client client_ctx;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];
struct mqtt_publish_param publish_param;

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

void publish_message(struct k_timer *timer_id)
{
    static int message_id = 0;

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = MQTT_MESSAGE;
    publish_param.message.payload.len = strlen(MQTT_MESSAGE);
    publish_param.message_id = message_id++;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Message published, message id: %d", publish_param.message_id);
    }
}

K_TIMER_DEFINE(publish_timer, publish_message, NULL);

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    k_timer_start(&publish_timer, K_SECONDS(5), K_SECONDS(5));

    memset(&client_ctx, 0, sizeof(client_ctx));
    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to broker, error: %d", ret);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
