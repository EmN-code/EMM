#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include <zephyr/net/mqtt.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

#define MQTT_PAYLOAD "work or not work"

/* MQTT client */
static struct mqtt_client client;
static struct sockaddr_storage broker;

/* Buffers for MQTT client */
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

/* Thread stack and handler for MQTT publish */
K_THREAD_STACK_DEFINE(mqtt_pub_stack, STACK_SIZE);
static struct k_thread mqtt_pub_thread;

/* Function to initialize the MQTT client */
static void mqtt_init(void) {
    mqtt_client_init(&client);

    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");

    client.protocol_version = MQTT_VERSION_3_1_0;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

/* Function to publish MQTT message */
static void mqtt_publish_work_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.topic.utf8 = (uint8_t *)"work_status";
    param.message.topic.topic.size = strlen("work_status");
    param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

/* Thread function for MQTT handling */
void mqtt_thread(void) {
    mqtt_init();

    int rc = mqtt_connect(&client);
    while (rc != 0) {
        printk("Failed to connect to MQTT broker, retrying...");
        k_sleep(K_SECONDS(2));
        rc = mqtt_connect(&client);
    }

    while (1) {
        mqtt_publish_work_state();
        k_sleep(K_SECONDS(5));
    }
}

void main(void) {
    k_thread_create(&mqtt_pub_thread, mqtt_pub_stack, STACK_SIZE,
                    (k_thread_entry_t)mqtt_thread, NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);
}