
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "zephyr_client"
#define MQTT_MESSAGE "work"

LOG_MODULE_REGISTER(main);

void mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client->broker = &broker;
    client->client_id.utf8 = MQTT_CLIENT_ID;
    client->client_id.size = strlen(MQTT_CLIENT_ID);
    client->protocol_version = MQTT_VERSION_3_1_1;

    int rc = mqtt_connect(client);
    if (rc) {
        LOG_ERR("Failed to connect to MQTT broker: %d", rc);
    } else {
        LOG_INF("Connected to MQTT broker");
    }
}

void mqtt_publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(client, &param);
    if (rc) {
        LOG_ERR("Failed to publish message: %d", rc);
    } else {
        LOG_INF("Message published");
    }
}

void main(void)
{
    struct mqtt_client client;
    uint8_t rx_buffer[100];
    uint8_t tx_buffer[100];

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
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
