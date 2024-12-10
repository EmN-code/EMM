
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

struct mqtt_client client;
struct sockaddr_storage broker;
struct mqtt_publish_param publish_param;
struct mqtt_topic topic;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
                publish_param.message.topic = topic;
                publish_param.message.payload.data = MQTT_PAYLOAD;
                publish_param.message.payload.len = MQTT_PAYLOAD_LEN;
                publish_param.message_id = 0;
                publish_param.dup_flag = MQTT_DUP;
                publish_param.retain_flag = MQTT_RETAIN;
                mqtt_publish(client, &publish_param);
            } else {
                printk("Failed to connect to MQTT broker\n");
            }
            break;
        case MQTT_EVT_PUBLISH:
            printk("Message published\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    int ret;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    mqtt_client_init(&client);

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        mqtt_publish(&client, &publish_param);
    }

    return 0;
}
