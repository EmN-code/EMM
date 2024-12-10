#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "esp32/state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client;
static struct sockaddr_storage broker_storage;
static struct sockaddr_in *broker = (struct sockaddr_in *)&broker_storage;

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 client_id = {.utf8 = (uint8_t *)MQTT_CLIENT_ID, .size = sizeof(MQTT_CLIENT_ID) - 1};

static void prepare_broker(void) {
    broker->sin_family = AF_INET;
    broker->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker->sin_addr);
}

static void connect_to_broker(void) {
    struct mqtt_sec_config sec_config;
    struct mqtt_utf8 password = {.utf8 = NULL};
    struct mqtt_utf8 user_name = {.utf8 = NULL};

    mqtt_client_init(&client);
    client.broker = (struct sockaddr *)broker;
    client.evt_cb = NULL;
    client.client_id = client_id;
    client.password = password;
    client.user_name = user_name;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

static void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
}

void main(void) {
    prepare_broker();
    connect_to_broker();

    while (true) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}