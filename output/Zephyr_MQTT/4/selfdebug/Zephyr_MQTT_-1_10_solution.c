
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

void mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr->sin_addr);

    mqtt_client_set_broker_address(client, (struct sockaddr *)broker_addr);

    int ret = mqtt_connect(client);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
    } else {
        LOG_INF("Connected to MQTT broker");
    }
}

void mqtt_publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = &topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = 1;
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
    struct mqtt_client_config config = {0};
    struct mqtt_client_init_options init_options = {0};

    mqtt_client_init(&client);
    mqtt_client_config_init(&config);
    mqtt_client_init_options_init(&init_options);

    mqtt_connect(&client);

    while (1) {
        mqtt_publish(&client);
        k_sleep(K_SECONDS(5));
    }
}
