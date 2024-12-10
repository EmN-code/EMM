#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>

#define BROKER_ADDR  "47.102.103.1"
#define BROKER_PORT  1883
#define PUB_TOPIC    "device/state"
#define CLIENT_ID    "esp32_device"
#define KEEP_ALIVE_INTERVAL 60
#define MQTT_BUFFER_SIZE 128

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_INF);

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[MQTT_BUFFER_SIZE];
static uint8_t tx_buffer[MQTT_BUFFER_SIZE];
static struct mqtt_utf8 client_id = {.utf8 = (uint8_t *)CLIENT_ID, .size = sizeof(CLIENT_ID) - 1};
static struct mqtt_utf8 pub_topic = {.utf8 = (uint8_t *)PUB_TOPIC, .size = sizeof(PUB_TOPIC) - 1};

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected");
            break;
        default:
            break;
    }
}

static void mqtt_init(void) {
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.protocol_version = MQTT_VERSION_3_1_1;

    broker_init();
}

static void mqtt_connect(void) {
    struct mqtt_sec_config *tls_config = NULL;

    struct mqtt_connect_client_info_t conn_info = {
        .broker = &broker,
        .client_id = &client_id,
        .mqtt_version = MQTT_VERSION_3_1_1,
        .clean_session = 1,
        .user_name = NULL,
        .password = NULL,
        .keep_alive = KEEP_ALIVE_INTERVAL,
    };

    mqtt_connect(&client, &conn_info, tls_config, K_NO_WAIT);
}

static void mqtt_publish_message(const char *message) {
    struct mqtt_publish_param param;
    struct mqtt_utf8 payload = {.utf8 = (uint8_t *)message, .size = strlen(message)};

    param.message.topic = pub_topic;
    param.message.payload = &payload;
    param.message.payload_len = payload.size;
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void) {
    mqtt_init();
    mqtt_connect();

    while (true) {
        mqtt_live(&client);
        mqtt_publish_message("work or not work");
        k_sleep(K_SECONDS(5));
    }
}
