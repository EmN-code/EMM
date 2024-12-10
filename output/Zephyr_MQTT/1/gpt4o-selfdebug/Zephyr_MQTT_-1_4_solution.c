#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state_topic"
#define MESSAGE "work or not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 client_id = {.utf8 = "esp32_client", .utf8_len = 11};

static void broker_init(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
}

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBLISH:
            printk("Message published!");
            break;
        default:
            break;
    }
}

static void mqtt_init(void) {
    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = client_id.utf8;
    client.client_id.size = client_id.utf8_len;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void mqtt_start(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = MESSAGE;
    param.message.payload.len = strlen(MESSAGE);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    while (1) {
        mqtt_publish(&client, &param);
        k_sleep(PUBLISH_INTERVAL);
    }
}

void main(void) {
    broker_init();
    mqtt_init();
    mqtt_connect(&client);
    mqtt_start();
}