#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 topic = {.utf8 = (uint8_t*)"device/state", .size = 12};
static struct mqtt_utf8 payload = {.utf8 = (uint8_t*)"work", .size = 4};
static struct mqtt_topic publish_topic;

void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

void mqtt_event_cb(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected!");
            break;
        default:
            break;
    }
}

void mqtt_setup(void) {
    struct mqtt_sec_config tls_config = { .peer_verify = 0 };

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_cb;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = 60;

    publish_topic.topic = topic;
    publish_topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_connect(&client);
}

void main(void) {
    int ret;
    broker_init();
    mqtt_setup();

    while (1) {
        ret = mqtt_publish(&client, &(struct mqtt_publish_param){
            .message.topic = publish_topic,
            .message.payload.data = payload.utf8,
            .message.payload.len = payload.size,
            .dup_flag = 0,
            .retain_flag = 0,
        });

        if (ret) {
            printk("Failed to publish message: %d", ret);
        } else {
            printk("Published message successfully");
        }

        k_sleep(PUBLISH_INTERVAL);
        mqtt_input(&client);
        mqtt_live(&client);
    }
}