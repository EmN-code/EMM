
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client;
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;
static struct mqtt_binstr payload;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected\n");
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

    payload.data = MQTT_PAYLOAD;
    payload.size = strlen(MQTT_PAYLOAD);

    publish_param.message.topic = topic;
    publish_param.message.payload = payload;
    publish_param.message_id = message_id++;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published\n");
    }
}

int main(void)
{
    struct sockaddr_in broker;
    struct mqtt_client_config config;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_addr.s_addr = inet_addr(MQTT_BROKER_ADDR);
    broker.sin_port = htons(MQTT_BROKER_PORT);

    memset(&config, 0, sizeof(config));
    config.client_id.utf8 = "zephyr_client";
    config.client_id.size = strlen("zephyr_client");
    config.user_name.utf8 = NULL;
    config.user_name.size = 0;
    config.password.utf8 = NULL;
    config.password.size = 0;
    config.keep_alive = 60;

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.config = &config;

    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }

    return 0;
}
