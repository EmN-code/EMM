
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const c,
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

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish, error: %d", ret);
    } else {
        LOG_INF("State published");
    }
}

void main(void)
{
    struct sockaddr_in broker;
    int ret;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }
}
