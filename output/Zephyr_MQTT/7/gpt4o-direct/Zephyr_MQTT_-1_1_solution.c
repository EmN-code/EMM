
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

LOG_MODULE_REGISTER(main);

static int mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_in broker;
    int ret;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    ret = mqtt_client_connect(client, (struct sockaddr *)&broker, sizeof(broker), MQTT_USERNAME, MQTT_PASSWORD, 0, NULL);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
    }

    return ret;
}

static void mqtt_publish(struct mqtt_client *client, const char *topic, const char *payload)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Published: %s", payload);
    }
}

void main(void)
{
    struct mqtt_client client;
    mqtt_client_init(&client);

    if (mqtt_connect(&client) != 0) {
        return;
    }

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, "work");
        k_sleep(K_SECONDS(5));
        mqtt_publish(&client, MQTT_TOPIC, "not work");
        k_sleep(K_SECONDS(5));
    }
}
