
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"

LOG_MODULE_REGISTER(main);

struct mqtt_client client_ctx;
K_THREAD_STACK_DEFINE(mqtt_thread_stack, 1024);
static struct k_thread mqtt_thread_data;

void mqtt_thread(void *arg1, void *arg2, void *arg3)
{
    int rc;
    struct mqtt_publish_param param;
    struct sockaddr_in broker;
    struct mqtt_client_config config = {0};

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    config.broker = &broker;
    config.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    config.client_id.size = strlen(MQTT_CLIENT_ID);

    mqtt_client_init(&client_ctx);
    client_ctx.config = &config;

    rc = mqtt_connect(&client_ctx);
    if (rc) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", rc);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));

        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = (uint8_t *)state;
        param.message.payload.len = strlen(state);
        param.message.dup = 0;
        param.message.qos = MQTT_QOS_1_AT_LEAST_ONCE;
        param.message.retain = 0;

        rc = mqtt_publish(&client_ctx, &param);
        if (rc) {
            LOG_ERR("Failed to publish message, error: %d", rc);
        } else {
            LOG_INF("Published state: %s", state);
        }
    }
}

int main(void)
{
    k_thread_create(&mqtt_thread_data, mqtt_thread_stack,
                    K_THREAD_STACK_SIZEOF(mqtt_thread_stack),
                    mqtt_thread, NULL, NULL, NULL,
                    K_PRIO_COOP(7), 0, K_NO_WAIT);

    return 0;
}
