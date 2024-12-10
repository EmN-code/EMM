#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_QOS_LEVEL MQTT_QOS_0_AT_MOST_ONCE

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static char payload[] = "work or not work";

static K_THREAD_STACK_DEFINE(mqtt_thread_stack, 1024);

void mqtt_evt_callback(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("[INFO] MQTT connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("[INFO] MQTT disconnected\n");
            break;
        default:
            break;
    }
}

void mqtt_thread(void) {
    int rc;

    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_callback;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.protocol_version = MQTT_VERSION_3_1_1;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("[ERROR] Connection failed\n");
        return;
    }

    while (1) {
        struct mqtt_publish_param pub_param;

        pub_param.message.topic.qos = MQTT_QOS_LEVEL;
        pub_param.message.payload.data = payload;
        pub_param.message.payload.len = strlen(payload);

        rc = mqtt_publish(&client, &pub_param);
        if (rc != 0) {
            printk("[ERROR] Publish failed: %d\n", rc);
        } else {
            printk("[INFO] Message published\n");
        }

        k_sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    printk("Starting MQTT example\n");
    k_thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                    (k_thread_entry_t)mqtt_thread, NULL, NULL, NULL, 7, 0, K_NO_WAIT);
    k_thread_start(mqtt_thread);

    return 0;
}