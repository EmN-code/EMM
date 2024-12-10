
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

void mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_storage broker;
    int ret;

    broker.ss_family = AF_INET;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    broker_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);

    ret = mqtt_client_connect(client, (struct sockaddr *)&broker_addr, sizeof(broker));
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
    } else {
        LOG_INF("Connected to MQTT broker");
    }
}

void mqtt_publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic;
    uint8_t payload[] = MQTT_PAYLOAD;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    param.message.topic = topic;
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
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
    uint8_t rx_buffer[100];
    uint8_t tx_buffer[100];
    struct mqtt_client_config config;

    config.client_id.utf8 = "zephyr_client";
    config.client_id.size = strlen("zephyr_client");
    config.broker = MQTT_BROKER_IP;
    config.keepalive = 60;
    config.clean_session = 1;

    mqtt_client_init(&client);
    client.config = &config;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);

    while (1) {
        mqtt_publish(&client);
        k_sleep(K_SECONDS(5));
    }
}
