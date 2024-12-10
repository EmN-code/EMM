#include <mqtt_client.h>
#include <logging/log.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];
static bool connected = false;

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                LOG_INF("MQTT client connected!");
                connected = true;
            } else {
                LOG_ERR("MQTT connect failed: %d", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            connected = false;
            break;
        default:
            break;
    }
}

static void connect_to_broker(void) {
    struct sockaddr_in broker;

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.rx_buf = rx_buffer;
    client.tx_buf = tx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    mqtt_connect(&client);
}

void main(void) {
    connect_to_broker();

    while (1) {
        if (connected) {
            struct mqtt_publish_param param;
            struct mqtt_publish_message message;

            message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
            message.topic.topic = "state";
            message.payload.data = "work or not work";
            message.payload.len = strlen("work or not work");
            param.message = &message;

            int rc = mqtt_publish(&client, &param);
            if (rc != 0) {
                LOG_ERR("Failed to publish message: %d", rc);
            } else {
                LOG_INF("Message published successfully!");
            }
        }

        k_sleep(K_SECONDS(5));
    }
}