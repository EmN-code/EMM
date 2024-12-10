#include <zephyr/kernel.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_device"
#define MQTT_TOPIC "device/state"
#define MQTT_MSG "work"

static struct mqtt_client mqtt_client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 client_id = MQTT_UTF8_LITERAL(MQTT_CLIENT_ID);

void prepare_mqtt_client(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&mqtt_client);

    mqtt_client.broker = &broker;
    mqtt_client.evt_cb = NULL;
    mqtt_client.client_id = client_id;
    mqtt_client.rx_buf = rx_buffer;
    mqtt_client.rx_buf_size = sizeof(rx_buffer);
    mqtt_client.tx_buf = tx_buffer;
    mqtt_client.tx_buf_size = sizeof(tx_buffer);
}

void mqtt_publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_MSG;
    param.message.payload.len = strlen(MQTT_MSG);
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&mqtt_client, &param);
}

void main(void) {
    prepare_mqtt_client();

    mqtt_connect(&mqtt_client);

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }
    mqtt_disconnect(&mqtt_client);
}