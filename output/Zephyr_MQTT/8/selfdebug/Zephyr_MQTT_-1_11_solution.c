
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"

LOG_MODULE_REGISTER(mqtt_client);

struct mqtt_client client_ctx;
K_THREAD_STACK_DEFINE(mqtt_stack, 2048);

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

void mqtt_publish_thread(void)
{
    while (1) {
        if (mqtt_client_is_connected(&client_ctx)) {
            struct mqtt_publish_param param;
            param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
            param.message.topic.topic.utf8 = MQTT_TOPIC;
            param.message.topic.topic.size = strlen(MQTT_TOPIC);
            param.message.payload.data = MQTT_PAYLOAD;
            param.message.payload.len = strlen(MQTT_PAYLOAD);
            param.message_id = sys_rand32_get();
            param.dup_flag = 0;
            param.retain_flag = 0;

            int ret = mqtt_publish(&client_ctx, &param);
            if (ret) {
                LOG_ERR("Failed to publish message: %d", ret);
            } else {
                LOG_INF("Message published");
            }
        }
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    struct sockaddr_in broker_addr;
    int ret;

    LOG_INF("Starting MQTT client");

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker_addr;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_mqtt_client";
    client_ctx.client_id.size = strlen(client_ctx.client_id.utf8);
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return ret;
    }

    k_thread_create(&mqtt_publish_thread, mqtt_stack,
                    K_THREAD_STACK_SIZEOF(mqtt_stack),
                    (k_thread_entry_t)mqtt_publish_thread,
                    NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

    return 0;
}
