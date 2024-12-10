#include <zephyr/kernel.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32client"
#define MQTT_TOPIC "state/report"
#define SLEEP_TIME_MS 5000

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_event_handler(struct mqtt_client *const cl, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("Connected to the broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from the broker\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("Message published\n");
            break;
        default:
            break;
    }
}

void mqtt_init() {
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

void mqtt_publish_state() {
    struct mqtt_publish_param param;
    struct mqtt_utf8 utf8_topic = {.utf8 = MQTT_TOPIC, .size = strlen(MQTT_TOPIC)};
    struct mqtt_utf8 utf8_message = {.utf8 = "work", .size = strlen("work")};

    param.message.topic = utf8_topic;
    param.message.payload.data = utf8_message.utf8;
    param.message.payload.len = utf8_message.size;
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void) {
    printk("Starting MQTT publisher\n");

    mqtt_init();

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to the broker\n");
        return;
    }

    while (1) {
        mqtt_publish_state();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}