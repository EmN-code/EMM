#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

static struct mqtt_client client;
static struct sockaddr_in broker;

static void prepare_mqtt_client(void) {
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");
}

static void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"esp32/status";
    param.message.topic.topic.size = strlen("esp32/status");
    param.message.payload.data = (uint8_t *)"work or not work";
    param.message.payload.len = strlen("work or not work");

    mqtt_publish(&client, &param);
    LOG_INF("Published: %s", log_strdup(param.message.payload.data));
}

void main(void) {
    prepare_mqtt_client();
    mqtt_connect(&client);

    while (1) {
        publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}
