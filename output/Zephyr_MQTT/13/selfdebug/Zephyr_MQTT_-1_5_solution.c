
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

void mqtt_publish_state(struct mqtt_client *client)
{
    static bool state = false;
    char payload[20];
    int ret;

    snprintf(payload, sizeof(payload), "%s", state ? "work" : "not work");
    state = !state;

    struct mqtt_publish_param param = {
        .message = {
            .topic = {
                .topic.utf8 = MQTT_TOPIC,
                .topic.size = strlen(MQTT_TOPIC),
            },
            .payload.data = payload,
            .payload.len = strlen(payload),
        },
        .dup_flag = 0,
        .retain_flag = 0,
        .message_id = 0,
        .qos = MQTT_QOS_0_AT_MOST_ONCE,
    };

    ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Published: %s", payload);
    }
}

void mqtt_thread(void)
{
    int ret;
    struct sockaddr_in broker;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    struct mqtt_client_config config = {
        .client_id = MQTT_CLIENT_ID,
        .broker = &broker,
        .evt_cb = mqtt_evt_handler,
    };

    mqtt_client_init(&client_ctx);
    client_ctx.config = &config;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        mqtt_publish_state(&client_ctx);
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    k_thread_create(&mqtt_thread_stack, 1024,
                    (k_thread_entry_t)mqtt_thread,
                    NULL, NULL, NULL,
                    K_PRIO_COOP(7), 0, K_NO_WAIT);

    return 0;
}
