
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0
#define MQTT_RETAIN 0

LOG_MODULE_REGISTER(main);

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

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = MQTT_PAYLOAD_LEN;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = MQTT_RETAIN;

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

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = MQTT_CLEAN_SESSION;
    client.keepalive = MQTT_KEEPALIVE;
    client.will_topic = NULL;
    client.will_message = NULL;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }
}
