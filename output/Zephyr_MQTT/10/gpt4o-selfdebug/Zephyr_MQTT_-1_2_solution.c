#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_PUB_INTERVAL K_SECONDS(5)

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static struct mqtt_client mqtt_client;
static struct sockaddr broker;

static struct mqtt_utf8 client_id = {
    .utf8 = (uint8_t *)MQTT_CLIENT_ID,
    .size = sizeof(MQTT_CLIENT_ID) - 1
};

void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt) {
    // Handle MQTT events if necessary, e.g., connection, publish acknowledgment.
}

void mqtt_connect(void) {
    broker_init();

    struct mqtt_sec_config sec_config;

    memset(&mqtt_client, 0, sizeof(mqtt_client));
    mqtt_client.broker = &broker;
    mqtt_client.client_id = client_id;
    mqtt_client.evt_cb = mqtt_evt_handler;
    mqtt_client.rx_buf = rx_buffer;
    mqtt_client.rx_buf_size = sizeof(rx_buffer);
    mqtt_client.tx_buf = tx_buffer;
    mqtt_client.tx_buf_size = sizeof(tx_buffer);
    mqtt_client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&mqtt_client);
}

void mqtt_publish_state(void) {
    struct mqtt_publish_param param;

    // Construct the message to publish
    struct mqtt_utf8 topic_str = {
        .utf8 = (uint8_t *)"state",
        .size = strlen("state")
    };

    struct mqtt_utf8 payload_str = {
        .utf8 = (uint8_t *)"work",
        .size = strlen("work")
    };

    memset(&param, 0, sizeof(param));
    param.message.topic = topic_str;
    param.message.payload = payload_str;
    param.message.payload_len = payload_str.size;
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_publish(&mqtt_client, &param);
}

void main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish_state();
        k_sleep(MQTT_PUB_INTERVAL);
    }
}