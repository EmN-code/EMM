
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
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
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = MQTT_PAYLOAD;
    message.payload.len = strlen(MQTT_PAYLOAD);
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
    struct mqtt_utf8 client_id;
    struct mqtt_transport transport;
    struct sockaddr_in broker;

    client_id.utf8 = "zephyr_client";
    client_id.size = strlen(client_id.utf8);

    transport.type = MQTT_TRANSPORT_NON_SECURE;
    transport.tcp.sock = -1;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);
    client.client_id = client_id;
    client.transport = transport;
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;

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
