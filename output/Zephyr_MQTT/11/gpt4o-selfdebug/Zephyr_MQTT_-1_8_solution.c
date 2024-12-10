#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "device_id"
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "password"
#define TOPIC "device/state"

/* MQTT message buffers */
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

/* MQTT client struct */
static struct mqtt_client client;

/* Broker address and port */
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const cli, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT broker connection failed %d\n", evt->result);
        } else {
            printk("Connected to MQTT broker\n");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    case MQTT_EVT_PUBLISH:
        printk("Message published\n");
        break;
    default:
        break;
    }
}

void publish_state(void)
{
    struct mqtt_publish_param param;

    memset(&param, 0, sizeof(param));
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = (uint8_t *)"work";
    param.message.payload.len = strlen("work");
    param.message_id = sys_rand32_get();

    mqtt_publish(&client, &param);

    printk("MQTT state published: work\n");
}

void main(void)
{
    int err;
    
    /* Initialize MQTT client */
    mqtt_client_init(&client);
    
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr.s_addr);

    struct mqtt_client_config mqtt_cfg = {
        .broker = (struct sockaddr *)&broker,
        .client_id.utf8 = (uint8_t *)MQTT_CLIENTID,
        .client_id.size = strlen(MQTT_CLIENTID),
        .user_name = {
            .utf8 = (uint8_t *)MQTT_USERNAME,
            .size = strlen(MQTT_USERNAME)
        },
        .password = {
            .utf8 = (uint8_t *)MQTT_PASSWORD,
            .size = strlen(MQTT_PASSWORD)
        },
        .evt_cb = mqtt_evt_handler,
        .rx_buf = rx_buffer,
        .rx_buf_size = sizeof(rx_buffer),
        .tx_buf = tx_buffer,
        .tx_buf_size = sizeof(tx_buffer),
    };

    err = mqtt_connect(&client);
    if (err) {
        printk("Failed to connect to MQTT broker, error: %d\n", err);
        return;
    }

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}