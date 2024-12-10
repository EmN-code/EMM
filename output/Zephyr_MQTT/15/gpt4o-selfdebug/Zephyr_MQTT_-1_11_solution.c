#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 client_id = MQTT_UTF8_LITERAL("zephyr_publisher");
static struct mqtt_utf8 topic = MQTT_UTF8_LITERAL("state");
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = "work or not work";

static struct k_work_delayable pub_work;

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBLISH:
            LOG_INF("Message published!");
            break;
        default:
            break;
    }
}

static void publish_state(struct k_work *work) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic = topic;
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);

    k_work_reschedule(&pub_work, K_SECONDS(5));
}

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

static void client_init(void) {
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)client_id.utf8;
    client.client_id.size = client_id.size;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void main(void) {
    broker_init();
    client_init();

    k_work_init_delayable(&pub_work, publish_state);

    mqtt_connect(&client);

    k_work_reschedule(&pub_work, K_SECONDS(5));

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_MSEC(500));
    }
}