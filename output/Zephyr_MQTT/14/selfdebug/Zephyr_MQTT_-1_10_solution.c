
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

LOG_MODULE_REGISTER(mqtt_client);

struct mqtt_client client_ctx;
struct mqtt_client_config client_cfg;
struct mqtt_client_buf client_buf;

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
    case MQTT_EVT_PUBLISH:
        LOG_INF("Message published");
        break;
    default:
        break;
    }
}

void publish_message(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Message published successfully");
    }
}

void main(void)
{
    int ret;
    struct sockaddr_in broker_addr;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&client_cfg, 0, sizeof(client_cfg));
    memset(&client_buf, 0, sizeof(client_buf));

    client_cfg.client_id.utf8 = "zephyr_mqtt_client";
    client_cfg.client_id.size = strlen(client_cfg.client_id.utf8);
    client_cfg.user_name = NULL;
    client_cfg.password = NULL;
    client_cfg.keepalive = 60;
    client_cfg.clean_session = 1;
    client_cfg.protocol_version = MQTT_VERSION_3_1_1;

    client_ctx.broker = &broker_addr;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_cfg = &client_cfg;
    client_ctx.client_buf = &client_buf;

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_message(&client_ctx);
    }
}
