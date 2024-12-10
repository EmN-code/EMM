
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

LOG_MODULE_REGISTER(mqtt_client);

struct mqtt_client client;
K_THREAD_STACK_DEFINE(mqtt_thread_stack, 2048);

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
        LOG_INF("Received PUBLISH message");
        break;
    default:
        break;
    }
}

void mqtt_publish_thread(void)
{
    struct mqtt_publish_param param;
    struct mqtt_client_config config;
    struct sockaddr_in broker;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    memset(&config, 0, sizeof(config));
    config.broker = &broker;
    config.client_id.utf8 = "esp32_client";
    config.client_id.size = strlen(config.client_id.utf8);
    config.evt_cb = mqtt_evt_handler;

    mqtt_client_init(&client);
    client.config = &config;

    int ret = mqtt_connect(&client);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));

        param.message.topic.topic.utf8 = MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = MQTT_MESSAGE;
        param.message.payload.len = strlen(MQTT_MESSAGE);
        param.message_id = 0;
        param.dup_flag = 0;
        param.retain_flag = 0;
        param.qos = MQTT_QOS_0_AT_MOST_ONCE;

        ret = mqtt_publish(&client, &param);
        if (ret) {
            LOG_ERR("Failed to publish message: %d", ret);
        } else {
            LOG_INF("Message published");
        }
    }
}

int main(void)
{
    k_thread_create(&mqtt_thread, mqtt_thread_stack,
                    K_THREAD_STACK_SIZEOF(mqtt_thread_stack),
                    (k_thread_entry_t)mqtt_publish_thread,
                    NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

    return 0;
}
