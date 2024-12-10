#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;

static struct mqtt_publish_param publish_param;

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

static void mqtt_evt_handler(struct mqtt_client *const cli, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        default:
            break;
    }
}

static void mqtt_init(void) {
    mqtt_client_init(&client);

    broker_init();

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"esp32_mqtt_client";
    client.client_id.size = strlen("esp32_mqtt_client");

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    client.rx_buf = malloc(128);
    client.rx_buf_size = 128;
    client.tx_buf = malloc(128);
    client.tx_buf_size = 128;
}

int main() {
    int rc;

    printk("Initializing MQTT client\n");

    mqtt_init();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker: %d\n", rc);
        return rc;
    }

    publish_param.message.topic.topic.utf8 = (uint8_t *)"state/topic";
    publish_param.message.topic.topic.size = strlen("state/topic");
    publish_param.message.payload.data = (uint8_t *)"work";
    publish_param.message.payload.len = strlen("work");
    publish_param.message_id = sys_rand32_get(); // Corrected member name
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;
    publish_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    while (1) {
        printk("Publishing message\n");
        rc = mqtt_publish(&client, &publish_param);
        if (rc != 0) {
            printk("Failed to publish message: %d\n", rc);
        }

        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);

    free(client.rx_buf);
    free(client.tx_buf);

    return 0;
}