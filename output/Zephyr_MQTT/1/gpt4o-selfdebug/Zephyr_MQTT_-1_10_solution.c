#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devices/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define BROKER_ADDR        "47.102.103.1"
#define BROKER_PORT        1883
#define PUBLISH_TOPIC      "esp32/state"
#define STATE_WORK         "work"
#define STATE_NOT_WORK     "not work"

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_utf8 publish_topic = {.utf8 = (uint8_t *)PUBLISH_TOPIC, .size = sizeof(PUBLISH_TOPIC)};
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBLISH:
            LOG_INF("Message published!");
            break;
        default:
            break;
    }
}

void mqtt_connect_broker(void) {
    struct mqtt_sec_config tls_config = {0};
    struct mqtt_connect_msg connect_msg = {
        .client_id = {.utf8 = (uint8_t *)"esp32-client", .size = (uint32_t)strlen("esp32-client")},
        .clean_session = 1,
        .user_name = NULL,
        .password = NULL
    };

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"esp32-client";
    client.client_id.size = strlen("esp32-client");
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    int rc = mqtt_connect(&client, &connect_msg, &tls_config);
    if (rc != 0) {
        LOG_ERR("MQTT connect error: %d", rc);
    }
}

void publish_state(const char *state) {
    struct mqtt_publish_param param;

    param.message.topic = publish_topic;
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.retain_flag = 0;
    param.message.dup_flag = 0;
    param.message_id = 0;

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        LOG_ERR("Failed to publish message: %d", rc);
    } else {
        LOG_INF("Published state: %s", state);
    }
}

void main(void) {
    LOG_INF("Starting MQTT publisher");
    mqtt_connect_broker();

    while (1) {
        publish_state(STATE_WORK);
        k_sleep(K_SECONDS(5));
        publish_state(STATE_NOT_WORK);
        k_sleep(K_SECONDS(5));
    }
}