#include <zephyr.h>
#include <net/mqtt.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_pub, LOG_LEVEL_DBG);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_ERR("MQTT client disconnected!");
        break;
    default:
        break;
    }
}

int init_mqtt(void) {
    int rc;
    broker_init();

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("Failed to connect to broker, error: %d", rc);
        return rc;
    }
    return 0;
}

void main(void) {
    LOG_INF("Starting MQTT sample");

    if (init_mqtt() != 0) {
        LOG_ERR("MQTT initialization failed");
        return;
    }

    while (1) {
        static const char *state = "work or not work";
        struct mqtt_publish_param param;

        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = "state";
        param.message.topic.topic.size = strlen("state");
        param.message.payload.data = state;
        param.message.payload.len = strlen(state);
        param.message_id = k_uptime_get_32();
        param.dup_flag = 0;
        param.retain_flag = 0;

        int rc = mqtt_publish(&client, &param);
        if (rc != 0) {
            LOG_ERR("Failed to publish message, error: %d", rc);
        } else {
            LOG_INF("Message published");
        }

        k_sleep(K_SECONDS(5));
    }
}