
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <errno.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_MESSAGE_ID 1

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_topic topic;
struct mqtt_topic_location topic_location;
struct mqtt_publish_message publish_message;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
            } else {
                printk("Failed to connect to MQTT broker: %d\n", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("Publish event received\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    int ret;
    struct sockaddr_in broker_addr;

    // Initialize MQTT client
    mqtt_client_init(&client);

    // Set up MQTT client parameters
    client.broker = (struct sockaddr *)&broker_addr;
    client.broker_len = sizeof(broker_addr);
    client.evt_cb = mqtt_evt_handler;

    // Set up broker address
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    // Connect to MQTT broker
    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    // Set up publish parameters
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic_location.topic = &topic;
    publish_message.topic = &topic_location;
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);
    publish_param.message = publish_message;
    publish_param.message_id = MQTT_MESSAGE_ID;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    while (1) {
        // Publish message
        ret = mqtt_publish(&client, &publish_param);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published\n");
        }

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
