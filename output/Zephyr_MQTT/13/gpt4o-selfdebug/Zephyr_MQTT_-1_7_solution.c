#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <string.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_CLIENTID "zephyr_client"
#define TOPIC "state_topic"
#define MESSAGE "work or not work"
#define STACK_SIZE 1024
#define PRIORITY 7

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = MESSAGE;

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!\n");
        break;
    case MQTT_EVT_PUBLISH:
        printk("Message published!\n");
        break;
    default:
        break;
    }
}

void connect_to_broker(void) {
    struct sockaddr *broker_addr = (struct sockaddr *)&broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = broker_addr;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);
}

void publish_state(void) {
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    pub_param.message.topic.topic.size = strlen(TOPIC);
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = sizeof(payload);
    pub_param.message_id = (uint16_t)sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    mqtt_publish(&client, &pub_param);
}

void main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}
