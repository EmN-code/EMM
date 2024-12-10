#include <zephyr/sys/printk.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;

void configure_mqtt(void) {
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = "zephyr_client_id";
    client.client_id.size = strlen(client.client_id.utf8);
}

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!
");
        break;
    default:
        printk("Unknown MQTT event %d
", evt->type);
        break;
    }
}

void main(void) {
    configure_mqtt();

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to broker!
");
        return;
    }

    while (1) {
        struct mqtt_publish_param param;
        struct mqtt_publish_message message;

        message.topic.qos = MQTT_QOS_0;
        message.topic.topic.utf8 = "device/state";
        message.topic.topic.size = strlen(message.topic.topic.utf8);
        message.payload.data = "work";
        message.payload.len = strlen("work");

        param.message = message;
        param.message_id = sys_rand32_get();

        mqtt_publish(&client, &param);

        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
}