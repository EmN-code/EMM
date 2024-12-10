#include <zephyr.h>
#include <stdlib.h>
#include <string.h>
#include <net/socket.h>

#define MQTT_CLIENTID "mqtt_client_esp32"
#define TOPIC "device/state"
#define PAYLOAD "work"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_in broker;

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT message published!");
        break;
    default:
        printk("Unhandled MQTT event type: %d", evt->type);
        break;
    }
}

void main(void)
{
    int err;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    while (1) {
        err = mqtt_connect(&client);
        if (err != 0) {
            printk("MQTT connect error: %d", err);
            k_sleep(K_SECONDS(5));
            continue;
        }

        struct mqtt_publish_param param;
        param.message.topic.topic.utf8 = TOPIC;
        param.message.topic.topic.size = strlen(TOPIC);
        param.message.payload.data = PAYLOAD;
        param.message.payload.len = strlen(PAYLOAD);
        param.message_id = sys_rand32_get();
        param.dup_flag = 0;
        param.retain_flag = 0;
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

        err = mqtt_publish(&client, &param);
        if (err != 0) {
            printk("Failed to publish message: %d", err);
        }

        mqtt_disconnect(&client);
        k_sleep(K_SECONDS(5));
    }
}