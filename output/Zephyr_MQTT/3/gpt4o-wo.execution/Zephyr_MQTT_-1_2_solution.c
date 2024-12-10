#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>

#include <stdio.h>
#include <stdlib.h>

#define MQTT_CLIENTID "esp32_client"
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;

void prepare_mqtt_client(void) {
    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
}

void publish_state(const char *state) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = (uint8_t *)state;
    param.message.payload.len = strlen(state);

    mqtt_publish(&client, &param);
}

static void mqtt_thread(void) {
    prepare_mqtt_client();
    mqtt_connect(&client);

    while (1) {
        publish_state("work");
        k_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
}

K_THREAD_DEFINE(mqtt_thread_id, 2048, mqtt_thread, NULL, NULL, NULL,
                7, 0, 0);

int main(void) {
    printf("Starting MQTT client\n");
    return 0;
}