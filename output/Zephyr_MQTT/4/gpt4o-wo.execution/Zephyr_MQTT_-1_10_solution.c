#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/linker/sections.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/random/random.h>
#include <zephyr/stats/stats.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/util.h>
#include <zephyr/tracing/tracing.h>

#include <zephyr/net/socket_poll.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/pm/state.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("MQTT message published\n");
            break;
        default:
            break;
    }
}

void main(void) {
    struct mqtt_client client;
    struct mqtt_publish_param param;
    struct sockaddr_in broker_addr;
    int rc;

    /* Initialize MQTT client */
    mqtt_client_init(&client);
    client.broker = (struct sockaddr *)&broker_addr;
    client.evt_cb = mqtt_event_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    /* Set broker address */
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    /* Connect to the broker */
    rc = mqtt_connect(&client);
    if (rc) {
        printk("MQTT connect failed: %d\n", rc);
        return;
    }

    /* Setup MQTT publish parameters */
    param.message.topic.qos = MQTT_QOS_1;
    param.retain_flag = 0;
    param.dup_flag = 0;
    sprintf(param.message.topic.topic.utf8, "test/topic");
    param.message.payload.data = "work or not work";
    param.message.payload.len = strlen("work or not work");

    while (1) {
        /* Publish message */
        rc = mqtt_publish(&client, &param);
        if (rc) {
            printk("MQTT publish failed: %d\n", rc);
        }

        k_sleep(K_SECONDS(5)); // Wait for 5 seconds

        mqtt_input(&client);
        mqtt_live(&client);
    }
}