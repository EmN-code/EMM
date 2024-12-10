
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <sys/printk.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

LOG_MODULE_REGISTER(mqtt_client);

struct mqtt_client client_ctx;
K_THREAD_STACK_DEFINE(mqtt_thread_stack, 2048);

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("Connection failed %d", evt->result);
        } else {
            LOG_INF("Connected");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected");
        break;
    default:
        break;
    }
}

void mqtt_publish_thread(void)
{
    struct mqtt_publish_param param;
    struct mqtt_client_config cfg;
    struct sockaddr_in broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    memset(&cfg, 0, sizeof(cfg));
    cfg.client_id.utf8 = "zephyr_mqtt_client";
    cfg.client_id.size = strlen(cfg.client_id.utf8);
    cfg.broker = &broker;
    cfg.evt_cb = mqtt_evt_handler;

    mqtt_client_init(&client_ctx);
    ret = mqtt_connect(&client_ctx, &cfg);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));

        param.message.topic.utf8 = MQTT_TOPIC;
        param.message.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = MQTT_MESSAGE;
        param.message.payload.len = strlen(MQTT_MESSAGE);
        param.message_id = 0;
        param.dup_flag = 0;
        param.retain_flag = 0;

        ret = mqtt_publish(&client_ctx, &param);
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
