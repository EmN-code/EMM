
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

LOG_MODULE_REGISTER(mqtt_client);

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
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
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = MQTT_MESSAGE;
    message.payload.len = strlen(MQTT_MESSAGE);
    message.qos = MQTT_QOS_0_AT_MOST_ONCE;

    param.message = message;
    param.message_id = 0; // Not used for QoS 0
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
    struct mqtt_utf8 client_id;
    struct mqtt_utf8 *user_name = NULL;
    struct mqtt_utf8 *password = NULL;
    struct mqtt_will_topic will_topic = {0};
    struct mqtt_will_msg will_msg = {0};

    client_id.utf8 = "zephyr_mqtt_client";
    client_id.size = strlen(client_id.utf8);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.client_id = client_id;
    client.user_name = user_name;
    client.password = password;
    client.will_topic = &will_topic;
    client.will_message = &will_msg;
    client.evt_cb = mqtt_event_handler;

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
