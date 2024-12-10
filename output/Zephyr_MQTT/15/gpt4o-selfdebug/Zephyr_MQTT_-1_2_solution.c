#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

static void client_init(void) {
    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = NULL; // Add your event callback if needed
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;

    static uint8_t rx_buffer[128];
    static uint8_t tx_buffer[128];
    static uint8_t payload_buf[128];

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.payload_buf = payload_buf;
    client.payload_buf_size = sizeof(payload_buf);
}

static int publish_message(struct mqtt_client *client, const char *state) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC_NAME;
    param.message.topic.topic.size = strlen(TOPIC_NAME);
    param.message.payload.data = (uint8_t *)state;
    param.message.payload.len = strlen(state);
    param.message_id = k_uptime_get_32();
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(client, &param);
}

void main(void) {
    int rc;
    const char *state = "work";

    broker_init();
    client_init();

    do {
        rc = mqtt_connect(&client);
        if (rc != 0) {
            printk("MQTT connect failed: %d\n", rc);
            k_sleep(K_SECONDS(1));
        }
    } while (rc != 0);

    while (1) {
        rc = publish_message(&client, state);
        if (rc != 0) {
            printk("Failed to publish message: %d\n", rc);
        } else {
            printk("Published state: %s\n", state);
        }
        k_sleep(PUBLISH_INTERVAL);
    }
}