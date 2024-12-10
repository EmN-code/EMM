#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(mqtt_example, LOG_LEVEL_INF);

#define BROKER_ADDR  "47.102.103.1"
#define BROKER_PORT  1883

#define CLIENT_ID    "zephyr_client"

static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];
static uint8_t payload[] = "work or not work";

static struct mqtt_client client;
static struct sockaddr_in broker;

static void connect_to_broker(void) {
    int ret;

    client.broker = &broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret != 0) {
        LOG_ERR("Failed to connect to broker: %d", ret);
        return;
    }

    LOG_INF("Connected to the broker");
}

static void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}