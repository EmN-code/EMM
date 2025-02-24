
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "zephyr_esp32"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4

struct mqtt_client client_ctx;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

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

void mqtt_connect(void)
{
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker_addr;
    client_ctx.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed: %d", ret);
    }
}

void publish_state(void)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = (uint8_t *)MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = MQTT_PAYLOAD_LEN;
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    int ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        LOG_ERR("MQTT publish failed: %d", ret);
    } else {
        LOG_INF("State published");
    }
}

void main(void)
{
    mqtt_connect();

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state();
    }
}
