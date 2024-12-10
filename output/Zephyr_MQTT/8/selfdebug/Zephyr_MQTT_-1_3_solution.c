
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60

LOG_MODULE_REGISTER(main);

struct mqtt_client client_ctx;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("Connected to MQTT broker");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("Disconnected from MQTT broker");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_client *client)
{
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = state_str;
    param.message.payload.len = strlen(state_str);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Published: %s", state_str);
    }

    state = !state;
}

void main(void)
{
    struct sockaddr_in broker;
    struct mqtt_client_config config;
    struct mqtt_client_init_param init;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    memset(&config, 0, sizeof(config));
    config.broker = &broker;
    config.client_id.utf8 = MQTT_CLIENT_ID;
    config.client_id.size = strlen(MQTT_CLIENT_ID);
    config.keepalive = MQTT_KEEPALIVE;

    memset(&init, 0, sizeof(init));
    init.client_id = &config.client_id;
    init.broker = config.broker;
    init.evt_cb = mqtt_evt_handler;
    init.rx_buf = rx_buffer;
    init.rx_buf_size = sizeof(rx_buffer);
    init.tx_buf = tx_buffer;
    init.tx_buf_size = sizeof(tx_buffer);

    mqtt_client_init(&client_ctx, &init);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx);
    }
}
