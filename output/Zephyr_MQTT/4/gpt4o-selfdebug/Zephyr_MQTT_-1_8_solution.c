#include <zephyr.h>
#include <sys/printk.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <random/rand32.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define PUBLISH_TOPIC "state"
#define PUBLISH_MESSAGE "work or not work"
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_BUFFER_SIZE 128

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[MQTT_BUFFER_SIZE];
static uint8_t tx_buffer[MQTT_BUFFER_SIZE];
static uint8_t payload[] = PUBLISH_MESSAGE;

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!\n");
        break;
    case MQTT_EVT_PUBACK:
        printk("Publishing complete (PUBACK received).\n");
        break;
    default:
        printk("Unknown MQTT event: %d\n", evt->type);
        break;
    }
}

void configure_mqtt(void)
{
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.user_name = (uint8_t *)MQTT_USERNAME;
    client.password = (uint8_t *)MQTT_PASSWORD;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void mqtt_publish_state(void)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)PUBLISH_TOPIC;
    param.message.topic.topic.size = strlen(PUBLISH_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
}

void main(void)
{
    configure_mqtt();

    mqtt_connect(&client);

    while (1) {
        mqtt_publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}