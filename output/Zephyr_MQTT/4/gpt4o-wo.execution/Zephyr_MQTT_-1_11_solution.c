#include <zephyr/syscalls/socket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/linker/sections.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/printk.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/poll.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static struct mqtt_client mqtt_client;
static struct sockaddr_in broker;

void configure_mqtt(void) {
    mqtt_client_init(&mqtt_client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client.broker = &broker;
    mqtt_client.evt_cb = mqtt_event_handler;
    mqtt_client.client_id.utf8 = "zephyr_client";
    mqtt_client.client_id.size = strlen(mqtt_client.client_id.utf8);
    mqtt_client.protocol_version = MQTT_VERSION_3_1_1;
}

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;

        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;

        default:
            break;
    }
}

void mqtt_publish_message(void) {
    struct mqtt_publish_param pub_param;
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = "/state";
    pub_param.message.topic.topic.size = strlen(pub_param.message.topic.topic.utf8);
    pub_param.message.payload.data = "work";
    pub_param.message.payload.len = strlen(pub_param.message.payload.data);

    mqtt_publish(&mqtt_client, &pub_param);
}

void main(void) {
    configure_mqtt();

    while (true) {
        mqtt_input(&mqtt_client);
        mqtt_live(&mqtt_client);
        mqtt_publish_message();
        k_sleep(K_SECONDS(5));
    }
}