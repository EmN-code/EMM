
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define SLEEP_TIME_MS 5000

static int mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client->broker = &broker;
    client->broker_len = sizeof(broker);

    return mqtt_connect(client);
}

static void mqtt_publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MQTT_MESSAGE;
    param.message.payload.len = strlen(MQTT_MESSAGE);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_publish(client, &param);
}

int main(void)
{
    struct mqtt_client client;
    mqtt_client_init(&client);

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        mqtt_publish(&client);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
