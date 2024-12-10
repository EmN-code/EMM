#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG "work"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[128];

void mqtt_event_cb(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("Connected to MQTT broker!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker!");
            break;
        default:
            break;
    }
}

void init_mqtt(void) {
    struct mqtt_sec_config *tls_config = NULL;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    mqtt_client_init(&client);

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_cb;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);
}

void send_message(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC_NAME;
    param.message.topic.topic.size = strlen(TOPIC_NAME);

    param.message.payload.data = MSG;
    param.message.payload.len = strlen(MSG);

    param.message_id = sys_rand32_get();

    mqtt_publish(&client, &param);
}

void mqtt_pub_thread(void) {
    init_mqtt();

    while (1) {
        send_message();
        k_sleep(K_SECONDS(5));
    }
}

K_THREAD_DEFINE(mqtt_thread_id, 1024, mqtt_pub_thread, NULL, NULL, NULL,
                7, 0, 0);

int main(void) {
    printk("MQTT application is starting\n");
    mqtt_pub_thread();
    return 0;
}