#include <time.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/arpa/inet.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define APP_SLEEP_MSECS 5000
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_PUBLISH_MESSAGE "work or not work"

struct mqtt_client client;

static void connect_to_broker(void)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &addr.sin_addr);

    struct mqtt_client_init_param client_init = {
        .broker = &addr,
        .evt_cb = NULL,
        .client_id.utf8 = (const uint8_t *)"client-id",
        .client_id.size = sizeof("client-id") - 1,
    };

    mqtt_client_init(&client, &client_init);
    mqtt_connect(&client);
}

static void publish_state(void)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_PUB_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_PUBLISH_MESSAGE;
    param.message.payload.len = strlen(MQTT_PUBLISH_MESSAGE);
    param.message_id = sys_rand32_get();

    mqtt_publish(&client, &param);
}

void main(void)
{
    printk("Starting MQTT Publisher\n");

    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }
}