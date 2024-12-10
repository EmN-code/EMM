
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
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

void publish_state(struct mqtt_client *client)
{
    static uint16_t message_id = 0;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);

    publish_param.message.topic = topic;
    publish_param.message.payload.data = MQTT_PAYLOAD;
    publish_param.message.payload.len = MQTT_PAYLOAD_SIZE;
    publish_param.message_id = message_id++;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        printk("Failed to publish message, error: %d\n", ret);
    } else {
        printk("Message published successfully\n");
    }
}

int main(void)
{
    int ret;
    struct sockaddr_in *broker_ptr = (struct sockaddr_in *)&broker;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker, error: %d\n", ret);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
