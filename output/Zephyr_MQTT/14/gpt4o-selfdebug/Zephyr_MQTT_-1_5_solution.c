#include <zephyr.h>
#include <random/rand32.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_STATE_INTERVAL K_SECONDS(5)

/* MQTT client configuration */
static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[20];

static struct mqtt_utf8 client_id = {
    .utf8 = (uint8_t *)"zephyr_client",
    .size = strlen("zephyr_client"),
};

static struct mqtt_utf8 topic = {
    .utf8 = (uint8_t *)"device/state",
    .size = strlen("device/state"),
};

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from broker\n");
        break;
    default:
        break;
    }
}

void connect_to_broker(void)
{
    struct mqtt_sec_config ssl_config;
    struct mqtt_transport transport = {
        .type = MQTT_TRANSPORT_NON_SECURE,
    };
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id = client_id;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport = transport;

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker (%d)\n", rc);
    }
}

void publish_state(void)
{
    struct mqtt_publish_param param;
    int rc;

    memset(&param, 0, sizeof(param));

    /* Create payload */
    snprintf(payload_buf, sizeof(payload_buf), "work");

    /* MQTT publish parameters */
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic = topic;
    param.message.payload.data = payload_buf;
    param.message.payload.len = strlen(payload_buf);

    rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        printk("Failed to publish message (%d)\n", rc);
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(PUB_STATE_INTERVAL);
    }

    return 0;
}