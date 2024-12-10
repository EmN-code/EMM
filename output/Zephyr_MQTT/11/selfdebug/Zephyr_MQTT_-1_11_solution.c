
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
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

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    static char payload[] = MQTT_PAYLOAD;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Message published");
    }
}

void main(void)
{
    struct mqtt_client client;
    struct sockaddr_in broker;
    uint8_t rx_buffer[100];
    uint8_t tx_buffer[100];
    int ret;

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = 1;
    client.keepalive = 60;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }
}
