
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0

LOG_MODULE_REGISTER(main);

struct mqtt_client client_ctx;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                LOG_INF("Connected to MQTT broker");
                // Publish a message after connection
                struct mqtt_publish_param param;
                param.message.topic.topic.utf8 = MQTT_TOPIC;
                param.message.topic.topic.size = strlen(MQTT_TOPIC);
                param.message.payload.data = MQTT_PAYLOAD;
                param.message.payload.len = MQTT_PAYLOAD_SIZE;
                param.message.payload.size = MQTT_PAYLOAD_SIZE;
                param.message.dup = 0;
                param.message.retain = 0;
                param.message.qos = MQTT_QOS;
                param.message_id = 0;
                param.cb = NULL;

                mqtt_publish(client, &param);
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

void main(void)
{
    struct sockaddr_in broker;
    int ret;

    // Initialize the MQTT client
    memset(&client_ctx, 0, sizeof(client_ctx));
    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.keepalive = MQTT_KEEPALIVE;
    client_ctx.clean_session = MQTT_CLEAN_SESSION;

    // Set up the broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    // Initialize the MQTT library
    ret = mqtt_init(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT initialization failed: %d", ret);
        return;
    }

    // Connect to the MQTT broker
    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connection failed: %d", ret);
        return;
    }

    // Publish a message every 5 seconds
    while (1) {
        k_sleep(K_SECONDS(5));

        struct mqtt_publish_param param;
        param.message.topic.topic.utf8 = MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = MQTT_PAYLOAD;
        param.message.payload.len = MQTT_PAYLOAD_SIZE;
        param.message.payload.size = MQTT_PAYLOAD_SIZE;
        param.message.dup = 0;
        param.message.retain = 0;
        param.message.qos = MQTT_QOS;
        param.message_id = 0;
        param.cb = NULL;

        ret = mqtt_publish(&client_ctx, &param);
        if (ret) {
            LOG_ERR("MQTT publish failed: %d", ret);
        } else {
            LOG_INF("Message published");
        }
    }
}
