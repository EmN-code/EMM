#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <net/net_core.h>
#include <net/net_if.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"
#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 client_id = {.utf8 = (const uint8_t *)"zephyr_client", .size = 13};

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch(evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT connected");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT disconnected");
            break;
        case MQTT_EVT_PUBACK:
            LOG_INF("Publish acknowledged");
            break;
        default:
            LOG_INF("Unhandled MQTT event %d", evt->type);
            break;
    }
}

void connect_to_broker(void) {
    struct mqtt_sec_config *tls_config = NULL;
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buffer = rx_buffer;
    client.rx_buffer_size = sizeof(rx_buffer);
    client.tx_buffer = tx_buffer;
    client.tx_buffer_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("MQTT connection failed (%d)", rc);
    }
}

void mqtt_publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (const uint8_t *)MQTT_PUB_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
    param.message.payload.data = (const uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        LOG_ERR("Failed to publish message (%d)", rc);
    }
}

void main(void) {
    connect_to_broker();

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }
}
