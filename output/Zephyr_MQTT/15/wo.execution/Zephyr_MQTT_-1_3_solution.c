
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <string.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_params;
struct mqtt_publish_message publish_msg;
struct mqtt_topic topic;

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
        default:
            break;
    }
}

int main(void)
{
    struct mqtt_connect_param connect_params;
    struct sockaddr_in broker_addr;

    memset(&client, 0, sizeof(client));
    memset(&connect_params, 0, sizeof(connect_params));
    memset(&publish_params, 0, sizeof(publish_params));
    memset(&publish_msg, 0, sizeof(publish_msg));
    memset(&topic, 0, sizeof(topic));

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    connect_params.client_id.utf8 = MQTT_CLIENT_ID;
    connect_params.client_id.size = strlen(MQTT_CLIENT_ID);
    connect_params.keepalive = 60;
    connect_params.clean_session = 1;

    mqtt_client_init(&client);
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;

    int ret = mqtt_connect(&client, &connect_params);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    publish_msg.topic = &topic;
    publish_msg.payload.data = MQTT_PAYLOAD;
    publish_msg.payload.len = strlen(MQTT_PAYLOAD);
    publish_msg.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_msg.retain = 0;

    publish_params.message = publish_msg;
    publish_params.message_id = 1;
    publish_params.dup_flag = 0;
    publish_params.retain_flag = 0;

    while (1) {
        ret = mqtt_publish(&client, &publish_params);
        if (ret) {
            printk("Failed to publish message: %d\n", ret);
        } else {
            printk("Message published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
