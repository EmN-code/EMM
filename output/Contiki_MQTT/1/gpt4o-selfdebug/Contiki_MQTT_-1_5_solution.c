#include <zephyr.h> // Required for thread management and delay
#include <net/mqtt.h> // Required for MQTT functionality
#include <net/socket.h> // Required for socket operations

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5) // 5 seconds

struct mqtt_client client;
static struct sockaddr_storage broker;

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *mqevt) {
    
}

void mqtt_connect(void) {
    struct mqtt_sec_config tls_config;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"nrf52840_client";
    client.client_id.size = strlen("nrf52840_client");

    mqtt_connect_noconfig(&client);
}

void mqtt_publish_state(void) {
    struct mqtt_publish_param param;

    memset(&param, 0, sizeof(param));
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"device/state";
    param.message.topic.topic.size = strlen("device/state");
    param.message.payload.data = (uint8_t *)"work";
    param.message.payload.len = strlen("work");

    mqtt_publish(&client, &param);
}

void main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}