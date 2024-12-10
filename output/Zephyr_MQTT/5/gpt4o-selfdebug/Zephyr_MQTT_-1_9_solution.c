#include <zephyr/types.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <device.h>
#include <drivers/gpio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUB_TOPIC "esp32/state"
#define MQTT_PAYLOAD "work or not work"
#define INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_connect_client_info_t client_info = {
    .client_id = CONFIG_MQTT_CLIENT_ID,
    .user_name = CONFIG_MQTT_USER_NAME,
    .password = CONFIG_MQTT_PASSWORD,
};

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT publish acknowledged!");
        break;
    default:
        break;
    }
}

void main(void) {
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_info = &client_info;

    mqtt_connect(&client);

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);

        struct mqtt_publish_param param;
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = MQTT_PUB_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
        param.message.payload.data = MQTT_PAYLOAD;
        param.message.payload.len = strlen(MQTT_PAYLOAD);
        param.message_id = sys_rand32_get();
        param.dup_flag = 0U;
        param.retain_flag = 0U;

        mqtt_publish(&client, &param);

        k_sleep(INTERVAL);
    }
}