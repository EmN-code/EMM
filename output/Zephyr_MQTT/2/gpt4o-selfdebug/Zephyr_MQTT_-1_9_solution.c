#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define TOPIC "state"
#define STATE_MESSAGE_WORK "work"
#define STATE_MESSAGE_NOT_WORK "not work"

/* Buffers for MQTT client. */
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static struct mqtt_client client;
static struct sockaddr broker;

/* Work status, toggled every 5 seconds. */
static bool work_status = true;

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT PUBLISH event.");
        break;
    case MQTT_EVT_PINGRESP:
        printk("MQTT PINGRESP event.");
        break;
    default:
        printk("Unhandled MQTT event %d", evt->type);
        break;
    }
}

static void connect_to_broker(void)
{
    struct mqtt_sec_config *tls_config = NULL;
    struct mqtt_utf8 client_id = { .utf8 = (uint8_t *)MQTT_CLIENTID, .size = strlen(MQTT_CLIENTID) };
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

static void publish_state_message(void)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (const uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = (work_status) ? STATE_MESSAGE_WORK : STATE_MESSAGE_NOT_WORK;
    param.message.payload.len = strlen((const char *)param.message.payload.data);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
    work_status = !work_status; // Toggle work status
}

void main(void)
{
    connect_to_broker();

    while (1) {
        publish_state_message();
        k_sleep(K_SECONDS(5));
    }
}