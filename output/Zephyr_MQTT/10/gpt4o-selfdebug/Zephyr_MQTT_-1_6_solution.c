#include <zephyr.h>
#include <mqtt.h>
#include <net/socket.h>
#include <string.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

#define WORK_STATE_TOPIC "device/state"

#define STACK_SIZE 2048

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt);

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[128];

void configure_mqtt_client() {
    mqtt_client_init(&client);

    broker.ss_family = AF_INET;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    inet_pton(AF_INET, BROKER_ADDR, &broker_addr->sin_addr);
    broker_addr->sin_port = htons(BROKER_PORT);    

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    struct mqtt_utf8 client_id = {.utf8 = (uint8_t *)"esp32_client", .size = strlen("esp32_client")};
    client.client_id = client_id;
}

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch(evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        default:
            break;
    }
}

void publish_state() {
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = (uint8_t *)WORK_STATE_TOPIC;
    param.message.topic.topic.size = strlen(WORK_STATE_TOPIC);
    param.message.payload.data = (uint8_t *)"work";
    param.message.payload.len = strlen("work");
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message_id = sys_rand32_get();

    mqtt_publish(&client, &param);
}

void main(void) {
    printk("MQTT simple publisher");

    configure_mqtt_client();

    mqtt_connect(&client);

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}