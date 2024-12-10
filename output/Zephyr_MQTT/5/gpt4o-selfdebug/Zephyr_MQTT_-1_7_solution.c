#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define MQTT_TOPIC "state/topic"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_utf8 client_id = {.utf8 = (uint8_t *)MQTT_CLIENTID, .size = sizeof(MQTT_CLIENTID) - 1};
static struct mqtt_utf8 topic = {.utf8 = (uint8_t *)MQTT_TOPIC, .size = sizeof(MQTT_TOPIC) - 1};
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static char payload[] = "work or not work";

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect error %d", evt->result);
        } else {
            LOG_INF("MQTT client connected!");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected %d", evt->result);
        break;
    case MQTT_EVT_PUBLISH:
        LOG_INF("MQTT client published!");
        break;
    default:
        break;
    }
}

void main(void)
{
    int ret;

    /* Initialize MQTT client */
    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id = client_id;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    /* Connect to MQTT broker */
    ret = mqtt_connect(&client);
    if (ret != 0) {
        LOG_ERR("Could not connect to MQTT broker: %d", ret);
        return;
    }

    /* Publish state every 5 seconds */
    while (1) {
        struct mqtt_publish_param param;
        param.message_id = 0;
        param.dup_flag = 0;
        param.retain_flag = 0;
        param.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.topic = topic;
        param.payload.data = payload;
        param.payload.len = strlen(payload);
        ret = mqtt_publish(&client, &param);
        if (ret != 0) {
            LOG_ERR("Could not publish message: %d", ret);
        } else {
            LOG_INF("Published: %s", payload);
        }
        k_sleep(PUBLISH_INTERVAL);
    }
}