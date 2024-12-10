#include <zephyr/syscall_listener.h>
#include <zephyr/syscalls/clock.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/interrupt_controller/intc_apic.h>
#include <zephyr/drivers/emul.h>
#include <zephyr/drivers/did.h>
#include <zephyr/drivers/dac.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/stats/stats.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
{
    // Handle different MQTT events
}

void main(void)
{
    struct mqtt_client client;
    struct sockaddr_in broker;

    // Configure MQTT broker
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;

    if (mqtt_connect(&client) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    while (1) {
        struct mqtt_publish_param pub_param;
        memset(&pub_param, 0, sizeof(pub_param));

        pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        pub_param.message.topic.topic.utf8 = (uint8_t *)MQTT_PUBLISH_TOPIC;
        pub_param.message.topic.topic.size = strlen(MQTT_PUBLISH_TOPIC);
        pub_param.message.payload.data = (uint8_t *)MQTT_MESSAGE;
        pub_param.message.payload.len = strlen(MQTT_MESSAGE);

        mqtt_publish(&client, &pub_param);

        // Sleep for 5 seconds
        usleep(5000000);
    }

    mqtt_disconnect(&client);
}