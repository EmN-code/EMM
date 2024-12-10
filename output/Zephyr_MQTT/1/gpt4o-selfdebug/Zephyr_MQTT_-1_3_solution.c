#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <random/rand32.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define CLIENT_ID "esp32_client"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

static u8_t rx_buffer[1024];
static u8_t tx_buffer[1024];
static u8_t payload[] = "work or not work";

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK: { /* Handle connection acknowledgement */ break; }
    case MQTT_EVT_DISCONNECT: { /* Handle disconnection */ break; }
    case MQTT_EVT_PUBLISH: { /* Handle incoming publish */ break; }
    case MQTT_EVT_PUBACK: { /* Handle publish acknowledgement */ break; }
    default: break;
    }
}

static void configure_mqtt(void)
{
    mqtt_client_init(&client);

    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &net_sin(&broker)->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (u8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

static void publish_state(struct k_timer *dummy)
{
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (u8_t *)"state/topic";
    pub_param.message.topic.topic.size = strlen("state/topic");
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = sizeof(payload);

    mqtt_publish(&client, &pub_param);
}

K_TIMER_DEFINE(publish_timer, publish_state, NULL);

void main(void)
{
    configure_mqtt();

    if (mqtt_connect(&client) != 0) {
        printk("MQTT connect error\n");
        return;
    }

    k_timer_start(&publish_timer, K_SECONDS(0), PUBLISH_INTERVAL);

    while (1) {
        mqtt_input(&client);
        k_sleep(K_MSEC(10));
    }
}
