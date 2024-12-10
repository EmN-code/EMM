#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state_topic"
#define PAYLOAD "work or not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(mqtt_publisher);

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_utf8 client_id = {.utf8 = (uint8_t *)CLIENT_ID, .size = sizeof(CLIENT_ID) - 1};
static struct mqtt_utf8 topic = {.utf8 = (uint8_t *)TOPIC, .size = sizeof(TOPIC) - 1};
static struct mqtt_utf8 payload = {.utf8 = (uint8_t *)PAYLOAD, .size = sizeof(PAYLOAD) - 1};
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
        } else {
            LOG_INF("MQTT client connected!");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected %d", evt->result);
        break;
    default:
        break;
    }
}

void mqtt_setup(void) {
    struct mqtt_sec_config *tls_config = NULL;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.tls_config = tls_config;
}

void mqtt_publish(struct mqtt_client *client) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = (uint8_t *)PAYLOAD;
    param.message.payload.len = strlen(PAYLOAD);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

void main(void) {
    LOG_INF("MQTT Publisher started");

    mqtt_setup();

    while (1) {
        int rc = mqtt_connect(&client);
        if (rc != 0) {
            LOG_ERR("MQTT connect failed: %d", rc);
            k_sleep(K_SECONDS(1));
            continue;
        }

        mqtt_publish(&client);

        k_sleep(PUBLISH_INTERVAL);
    }
}