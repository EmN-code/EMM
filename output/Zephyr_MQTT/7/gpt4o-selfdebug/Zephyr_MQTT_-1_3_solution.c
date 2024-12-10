#include <zephyr.h>
#include <net/socket.h>
#include <net/net_ip.h>
#include <net/mqtt.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "state"
#define MQTT_MSG "work"
#define SLEEP_TIME K_SECONDS(5) // Sleep time in milliseconds

static struct mqtt_client client;
static uint8_t mqtt_rx_buffer[128];
static uint8_t mqtt_tx_buffer[128];

static void prepare_mqtt_client(void) {
    struct sockaddr_in broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = mqtt_rx_buffer;
    client.rx_buf_size = sizeof(mqtt_rx_buffer);
    client.tx_buf = mqtt_tx_buffer;
    client.tx_buf_size = sizeof(mqtt_tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void mqtt_publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_PUB_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_MSG;
    param.message.payload.len = strlen(MQTT_MSG);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void) {
    prepare_mqtt_client();
    mqtt_connect(&client);

    while (1) {
        mqtt_input(&client);
        mqtt_publish_state();
        k_sleep(SLEEP_TIME);
    }
}