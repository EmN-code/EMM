#include <zephyr/net/wifi.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static struct sockaddr_in broker;

void publish_state(void)
{
    struct mqtt_publish_param param;
    const char *state_message = "work or not work";
    
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "state";
    param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
    param.message.payload.data = state_message;
    param.message.payload.len = strlen(state_message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    default:
        break;
    }
}

void main(void)
{
    broker_init();

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "esp32_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    while (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker, retrying...");
        k_sleep(K_SECONDS(1));
    }

    while (1) {
        publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}