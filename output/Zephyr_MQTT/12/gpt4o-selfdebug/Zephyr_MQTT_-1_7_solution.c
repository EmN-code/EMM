#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(mqtt_example, LOG_LEVEL_INF);

#define CONNECT_DELAY K_SECONDS(5)
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

#define MQTT_CLIENTID "zephyr_client"
#define TOPIC "state"
#define PAYLOAD "work or not work"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[128];

static void prepare_mqtt_message(struct mqtt_publish_param *param) {
    param->message.payload.data = PAYLOAD;
    param->message.payload.len = strlen(PAYLOAD);
    param->message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param->message.topic.topic.utf8 = TOPIC;
    param->message.topic.topic.size = strlen(TOPIC);
    param->dup_flag = 0U;
    param->retain_flag = 0U;
}

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("Connected to MQTT broker");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("Disconnected from MQTT broker");
            break;
        default:
            break;
    }
}

static void mqtt_setup(void) {
    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void main(void) {
    int rc;
    struct mqtt_publish_param param;

    LOG_INF("Starting MQTT client");

    broker_init();
    mqtt_setup();

    while (1) {
        rc = mqtt_connect(&client);
        if (rc != 0) {
            LOG_ERR("mqtt_connect error: %d", rc);
            k_sleep(CONNECT_DELAY);
            continue;
        }

        prepare_mqtt_message(&param);

        while (1) {
            mqtt_input(&client);
            mqtt_live(&client);
            
            rc = mqtt_publish(&client, &param);
            if (rc != 0) {
                LOG_ERR("mqtt_publish error: %d", rc);
                break;
            }

            k_sleep(K_SECONDS(5));
        }

        mqtt_disconnect(&client);
        k_sleep(CONNECT_DELAY);
    }
}