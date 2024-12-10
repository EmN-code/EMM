#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 client_id = {.utf8 = "zephyr_client", .size = 13};
static struct mqtt_utf8 topic = {.utf8 = "state", .size = 5};
static struct mqtt_utf8 payload;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static struct addrinfo *broker_addr;
static bool connected = false;

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            connected = true;
            LOG_INF("MQTT client connected!");
        } else {
            LOG_ERR("MQTT connect failed: %d", evt->result);
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected!");
        connected = false;
        break;
    default:
        LOG_DBG("Unhandled MQTT event type: %d", evt->type);
        break;
    }
}

static void connect_to_broker(void)
{
    struct mqtt_sec_config *tls_config = NULL;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id = client_id;
    client.password = NULL;
    client.user_name = NULL;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client);
    if (ret != 0) {
        LOG_ERR("MQTT connect failed: %d", ret);
    }
}

static void publish_state(bool state)
{
    const char *payload_str = state ? "work" : "not work";
    payload.utf8 = (uint8_t *)payload_str;
    payload.size = strlen(payload_str);

    struct mqtt_publish_param param;
    param.message.topic = topic;
    param.message.payload.data = payload.utf8;
    param.message.payload.len = payload.size;
    param.message_id = k_uptime_get_32();
    param.message.dup_flag = 0U;
    param.message.retain_flag = 0U;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    int ret = mqtt_publish(&client, &param);
    if (ret < 0) {
        LOG_ERR("Failed to publish, error: %d", ret);
    } else {
        LOG_INF("Published: %s", payload_str);
    }
}

void main(void)
{
    LOG_INF("Starting MQTT publisher");
    connect_to_broker();
    while (!connected) {
        k_sleep(K_MSEC(100));
    }

    while (1) {
        publish_state(true);
        k_sleep(PUB_INTERVAL);
        publish_state(false);
        k_sleep(PUB_INTERVAL);
    }
}
