#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define MQTT_TOPIC "esp32/state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"

struct mqtt_client client;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT connect failed: %d
", evt->result);
        }
        break;

    case MQTT_EVT_PUBACK:
        if (evt->result != 0) {
            printk("MQTT PUBACK error: %d
", evt->result);
        }
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected
");
        break;

    default:
        break;
    }
}

void publisher_thread(void)
{
    struct mqtt_publish_param param;
    int rc;

    while (true) {
        /* Prepare the message */
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = (uint8_t *)MQTT_PAYLOAD_WORK;
        param.message.payload.len = strlen(MQTT_PAYLOAD_WORK);
        param.message_id = k_uptime_get_32();
        param.dup_flag = 0U;
        param.retain_flag = 0U;

        /* Publish the message */
        rc = mqtt_publish(&client, &param);
        if (rc != 0) {
            printk("Failed to publish message: %d
", rc);
        }

        /* Wait for 5 seconds */
        k_sleep(K_SECONDS(5));
    }
}

void main(void)
{
    int rc;
    struct mqtt_client_init_param client_init;

    /* Initialize the MQTT client */
    client_init.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client_init.client_id.size = strlen(MQTT_CLIENTID);
    client_init.broker = (struct sockaddr *)&broker;
    client_init.evt_cb = mqtt_evt_handler;
    client_init.rx_buf = NULL;
    client_init.rx_buf_size = 0u;
    client_init.tx_buf = NULL;
    client_init.tx_buf_size = 0u;

    /* Set broker details */
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    rc = mqtt_client_init(&client, &client_init);
    if (rc != 0) {
        printk("MQTT client init error: %d
", rc);
        return;
    }

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("MQTT connect error: %d
", rc);
        return;
    }

    k_thread_create(&publisher_thread_data, publisher_thread_stack,
                    K_THREAD_STACK_SIZEOF(publisher_thread_stack),
                    publisher_thread, NULL, NULL, NULL,
                    K_PRIO_PREEMPT(7), 0, K_NO_WAIT);
}