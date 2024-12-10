
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;

static void mqtt_evt_handler(struct mqtt_client *const c,
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

static int mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_in broker;
    int ret;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client->broker = (struct sockaddr *)&broker;
    client->broker_len = sizeof(broker);

    ret = mqtt_connect(client);
    if (ret) {
        printk("MQTT connect failed: %d\n", ret);
    }

    return ret;
}

static void mqtt_publish(struct mqtt_client *client)
{
    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = strlen(MQTT_PAYLOAD);

    publish_param.message = publish_message;
    publish_param.message_id = sys_rand32_get();
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        printk("MQTT publish failed: %d\n", ret);
    }
}

int main(void)
{
    int ret;

    mqtt_client_init(&client);

    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret) {
        return ret;
    }

    while (1) {
        mqtt_publish(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
