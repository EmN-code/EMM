
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <string.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE_SEC 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

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

int main(void)
{
    int ret;
    struct sockaddr_in broker;
    struct mqtt_connect_param connect_params;
    struct mqtt_publish_param publish_params;
    struct mqtt_publish_message publish_message;

    memset(&client, 0, sizeof(client));
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE_SEC;
    client.clean_session = MQTT_CLEAN_SESSION;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    connect_params.client_id.utf8 = "zephyr_client";
    connect_params.client_id.size = strlen("zephyr_client");
    connect_params.user_name = NULL;
    connect_params.password = NULL;
    connect_params.clean_session = MQTT_CLEAN_SESSION;
    connect_params.keepalive = MQTT_KEEPALIVE_SEC;
    connect_params.will_topic = NULL;
    connect_params.will_message = NULL;

    ret = mqtt_connect(&client, &connect_params);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = MQTT_PAYLOAD_SIZE;
    publish_message.qos = MQTT_QOS;
    publish_message.retain = 0;

    publish_params.message = publish_message;
    publish_params.message_id = sys_rand32_get();
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
