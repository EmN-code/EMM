
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS MQTT_QOS_0_AT_MOST_ONCE

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 client_id = {
    .utf8 = MQTT_CLIENT_ID,
    .size = sizeof(MQTT_CLIENT_ID) - 1
};

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
        } else {
            LOG_INF("MQTT connected!");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT disconnected!");
        break;
    default:
        break;
    }
}

static int mqtt_connect(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker;
    client_ctx.client_id = &client_id;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);
    client_ctx.keepalive = MQTT_KEEPALIVE;
    client_ctx.clean_session = MQTT_CLEAN_SESSION;

    return mqtt_connect(&client_ctx);
}

static int mqtt_publish(const char *payload)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = sizeof(MQTT_TOPIC) - 1
    };

    param.message.topic = &topic;
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message.qos = MQTT_QOS;
    param.message.retained = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(&client_ctx, &param);
}

void main(void)
{
    int ret;

    ret = mqtt_connect();
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        ret = mqtt_publish("work");
        if (ret) {
            LOG_ERR("Failed to publish message: %d", ret);
        } else {
            LOG_INF("Message published successfully");
        }
    }
}
