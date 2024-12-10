#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define MQTT_TOPIC "esp32/status"
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"
#define INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    /* Handle different MQTT events here */
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!\n");
        break;
    default:
        break;
    }
}

static void prepare_mqtt_client(void)
{
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void publish_state(bool work_state)
{
    struct mqtt_publish_param param;
    const char *state = work_state ? STATE_WORK : STATE_NOT_WORK;

    memset(&param.message.topic, 0, sizeof(param.message.topic));
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);

    param.message.payload.data = (uint8_t *)state;
    param.message.payload.len = strlen(state);

    param.message_id = sys_rand32_get();
    param.dup_flag = false;
    param.retain_flag = false;

    mqtt_publish(&client, &param);
    printk("Published: %s\n", state);
}

void main(void)
{
    bool work_state = true;
    int err;

    prepare_mqtt_client();

    printk("Connecting to MQTT broker...\n");
    err = mqtt_connect(&client);
    if (err) {
        printk("Failed to connect to broker, error: %d\n", err);
        return;
    }

    while (1) {
        publish_state(work_state);
        work_state = !work_state;

        /* Simulate waiting operation */
        k_sleep(INTERVAL);
    }
}