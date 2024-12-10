#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>

/* Define Broker details */
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define STACK_SIZE 4096
#define PRIORITY 7

#define STATE_TOPIC "device/state"

/* MQTT buffers */
static uint8_t rx_buffer[1024];
static uint8_t tx_buffer[1024];
static uint8_t payload_buf[256];

/* MQTT client structure */
static struct mqtt_client client;
static struct sockaddr_in broker;

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_INF);

void broker_init(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBLISH:
            LOG_INF("MQTT PUBLISH received!");
            break;
        default:
            break;
    }
}

void connect_to_broker(void) {
    struct mqtt_sec_config tls_config = { 0 };

    struct mqtt_client_init_param params = {
        .broker = &broker,
        .evt_cb = mqtt_evt_handler,
        .client_id = MQTT_CLIENTID_SIZE,
        .user_name = NULL,
        .password = NULL,
        .transport.type = MQTT_TRANSPORT_NON_SECURE,
        .rx_buf = rx_buffer,
        .rx_buf_size = ARRAY_SIZE(rx_buffer),
        .tx_buf = tx_buffer,
        .tx_buf_size = ARRAY_SIZE(tx_buffer),
    };

    mqtt_client_init(&client);
    mqtt_client_cfg_set(&client, &params);
    mqtt_connect(&client);
}

void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = STATE_TOPIC;
    param.message.topic.topic.size = strlen(STATE_TOPIC);
    param.message.payload.data = payload_buf;
    param.message.payload.len = snprintf(payload_buf, sizeof(payload_buf), "work");
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void) {
    broker_init();
    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}