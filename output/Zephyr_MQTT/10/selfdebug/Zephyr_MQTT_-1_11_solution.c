
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_client);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

static int mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_storage broker;
    int ret;

    broker.sa_family = AF_INET;
    ((struct sockaddr_in *)&broker)->sin_port = htons(MQTT_BROKER_PORT);
    ret = inet_pton(AF_INET, MQTT_BROKER_ADDR, &((struct sockaddr_in *)&broker)->sin_addr);
    if (ret < 0) {
        LOG_ERR("Invalid address");
        return ret;
    }

    client->broker = &broker;
    client->evt_cb = NULL;
    client->client_id.utf8 = "esp32_client";
    client->client_id.size = strlen("esp32_client");

    ret = mqtt_connect(client);
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return ret;
    }

    return 0;
}

static void mqtt_publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param;

    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret < 0) {
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
    int ret;

    memset(&client, 0, sizeof(client));
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret < 0) {
        LOG_ERR("MQTT connect failed: %d", ret);
        return;
    }

    while (1) {
        mqtt_publish(&client);
        k_sleep(K_SECONDS(5));
    }
}
