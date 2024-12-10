#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/devices/gpio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL K_SECONDS(5)
#define MQTT_BUFFER_SIZE 128

static uint8_t tx_buffer[MQTT_BUFFER_SIZE];
static uint8_t rx_buffer[MQTT_BUFFER_SIZE];
static struct mqtt_client client;
static struct sockaddr broker;

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        default:
            break;
    }
}

void connect_to_broker(void) {
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to broker.\n");
    }
}

void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "device/state";
    param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
    param.message.payload.data = "work or not work";
    param.message.payload.len = strlen(param.message.payload.data);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    if (mqtt_publish(&client, &param) != 0) {
        printk("Failed to publish state.\n");
    }
}

int main(void) {
    broker.sa_family = AF_INET;
    net_sin(&broker)->sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDR, &net_sin(&broker)->sin_addr);

    mqtt_client_init(&client);

    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(PUB_INTERVAL);
    }

    return 0;
}