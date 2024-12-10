
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <string.h>
#include <net/net_ip.h>
#include <net/net_core.h>
#include <net/net_context.h>
#include <net/net_pkt.h>
#include <net/net_l2.h>
#include <net/net_stats.h>
#include <net/net_timeout.h>
#include <net/net_ip.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static struct mqtt_topic topic;

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to MQTT broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    case MQTT_EVT_PUBACK:
        printk("Message published\n");
        break;
    default:
        break;
    }
}

void publish_state(struct mqtt_client *client)
{
    static uint16_t message_id = 0;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    publish_message.topic = &topic;
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);
    publish_message.qos = MQTT_QOS;

    publish_param.message = publish_message;
    publish_param.message_id = message_id++;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        printk("Failed to publish message, error: %d\n", ret);
    }
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker, error: %d\n", ret);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }

    return 0;
}
