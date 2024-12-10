#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "esp32/state"
#define SLEEP_TIME_MS 5000

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t mqtt_rx_buffer[128];
static uint8_t mqtt_tx_buffer[128];
static uint8_t payload[] = "work or not work";

static void publish(struct mqtt_client *client, enum mqtt_qos qos, uint8_t *data, size_t len) {
    struct mqtt_publish_param param;

    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = data;
    param.message.payload.len = len;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBLISH:
            printk("MQTT PUBLISH event received!");
            break;
        case MQTT_EVT_PUBACK:
            printk("MQTT PUBLISH acknowledged!");
            break;
        default:
            printk("MQTT event: %d", evt->type);
            break;
    }
}

static int mqtt_init(void) {
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = mqtt_rx_buffer;
    client.rx_buf_size = sizeof(mqtt_rx_buffer);
    client.tx_buf = mqtt_tx_buffer;
    client.tx_buf_size = sizeof(mqtt_tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    return mqtt_connect(&client);
}

void main(void) {
    int err;

    err = mqtt_init();
    if (err) {
        printk("Failed to initialize MQTT client: %d", err);
        return;
    }

    while (1) {
        publish(&client, MQTT_QOS_0_AT_MOST_ONCE, payload, sizeof(payload));
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}