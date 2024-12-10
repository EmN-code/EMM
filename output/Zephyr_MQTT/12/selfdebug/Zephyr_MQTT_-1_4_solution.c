
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(mqtt_client);

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

void publish_state(struct mqtt_client *client)
{
    static const char *payload = MQTT_PAYLOAD;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC)
    };

    publish_param.message.topic = topic;
    publish_param.message.payload.data = (uint8_t *)payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Message published");
    }
}

void main(void)
{
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    struct mqtt_client_config config = {
        .broker = &broker_addr,
        .client_id.utf8 = "esp32_client",
        .client_id.size = strlen("esp32_client"),
        .evt_cb = mqtt_evt_handler,
        .rx_buf = rx_buffer,
        .rx_buf_size = sizeof(rx_buffer),
        .tx_buf = tx_buffer,
        .tx_buf_size = sizeof(tx_buffer),
        .keepalive = 60
    };

    mqtt_client_init(&client_ctx);
    int ret = mqtt_connect(&client_ctx, &config);
    if (ret) {
        LOG_ERR("Failed to connect to broker, error: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
