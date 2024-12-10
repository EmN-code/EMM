#include <net/mqtt.h>
#include <net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "esp32/state"
#define BUFFER_SIZE 128
#define MQTT_KEEPALIVE 60
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 password;
static struct mqtt_utf8 username;
static uint8_t tx_buffer[BUFFER_SIZE];
static uint8_t rx_buffer[BUFFER_SIZE];
static struct mqtt_publish_param pub_param;

void data_publish(struct mqtt_client *client)
{
    static char payload[] = "work or not work";
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = sizeof(payload) - 1;
    pub_param.message.topic.topic.utf8 = PUB_TOPIC;
    pub_param.message.topic.topic.size = strlen(PUB_TOPIC);
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message_id++;
    int err = mqtt_publish(client, &pub_param);
    if (err) {
        LOG_ERR("Failed to publish, error: %d", err);
    } else {
        LOG_INF("Data published to topic %s", PUB_TOPIC);
    }
}

void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result) {
            LOG_ERR("MQTT connect failed: %d", evt->result);
            break;
        }
        LOG_INF("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_ERR("MQTT client disconnected %d", evt->result);
        break;
    case MQTT_EVT_PUBLISH:
        LOG_INF("MQTT publish complete.");
        break;
    default:
        LOG_INF("Unknown MQTT event type: %d", evt->type);
        break;
    }
}

int main(void)
{
    broker_init();
    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int err = mqtt_connect(&client);
    if (err) {
        LOG_ERR("Failed to connect to broker: %d", err);
        return 1;
    }

    while (1) {
        data_publish(&client);
        k_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}