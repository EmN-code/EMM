#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <net/aws_credentials.h>
#include <settings/settings.h>
#include <random/rand32.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_CLIENT_ID "esp32-zephyr"
#define MQTT_TOPIC "state"

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static struct mqtt_client client;

/* Function prototype for connecting to the MQTT broker */
static int mqtt_connect(struct mqtt_client *client);

/* Function prototype for publishing a message */
static int mqtt_publish_message(struct mqtt_client *client, const char *topic, const char *payload);

void main(void) {
    int err;

    /* Initialize the MQTT client */
    memset(&client, 0x00, sizeof(client));
    mqtt_client_init(&client);

    client.broker = MQTT_BROKER_IP;
    client.broker_port = MQTT_BROKER_PORT;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    /* Connect to the broker */
    err = mqtt_connect(&client);
    if (err) {
        printk("Failed to connect to the broker, error %d\n", err);
        return;
    }

    while (1) {
        printk("Publishing message to the topic\n");
        err = mqtt_publish_message(&client, MQTT_TOPIC, "work");
        if (err) {
            printk("Failed to publish message, error %d\n", err);
        }

        k_sleep(PUBLISH_INTERVAL);
    }
}

static int mqtt_connect(struct mqtt_client *client) {
    struct sockaddr_in broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    /* Connect */
    return mqtt_connect(client);
}

static int mqtt_publish_message(struct mqtt_client *client, const char *topic, const char *payload) {
    struct mqtt_publish_param pub_param;

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)topic;
    pub_param.message.topic.topic.size = strlen(topic);
    pub_param.message.payload.data = (uint8_t *)payload;
    pub_param.message.payload.len = strlen(payload);

    pub_param.message_id = sys_rand32_get();/* Random message ID */
    pub_param.retain_flag = 0;
    pub_param.dup_flag = 0;

    return mqtt_publish(client, &pub_param);
}