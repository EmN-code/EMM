
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <stdio.h>
#include <string.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(mqtt_client);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

struct mqtt_client client;
K_THREAD_STACK_DEFINE(mqtt_thread_stack, 1024);

void mqtt_event_handler(struct mqtt_client *const client,
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

void mqtt_publish_thread(void)
{
    struct mqtt_publish_param param;
    struct mqtt_client_config config;
    struct sockaddr_in broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    memset(&config, 0, sizeof(config));
    config.broker = &broker;
    config.client_id.utf8 = "esp32_client";
    config.client_id.size = strlen(config.client_id.utf8);

    mqtt_client_init(&client);
    client.config = &config;
    client.evt_cb = mqtt_event_handler;

    ret = mqtt_connect(&client);
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

        ret = mqtt_publish(&client, &param);
        if (ret) {
            LOG_ERR("Failed to publish message: %d", ret);
        } else {
            LOG_INF("Published message: %s", MQTT_MESSAGE);
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
