#include <zephyr/types.h>
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32-client"

struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static struct mqtt_publish_param pub_param;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            printk("MQTT client connected!");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT message published!");
        break;
    default:
        break;
    }
}

void configure_broker(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);
}

void main(void)
{
    printk("Starting MQTT sample...");

    configure_broker();

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    while (mqtt_connect(&client)) {
        printk("Failed to connect, retrying...");
        k_sleep(K_SECONDS(1));
    }

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = "test/topic";
    pub_param.message.topic.topic.size = strlen("test/topic");
    pub_param.message.payload.data = "work";
    pub_param.message.payload.len = strlen("work");
    pub_param.retain_flag = 0;

    while (1) {
        int rc = mqtt_publish(&client, &pub_param);
        if (rc) {
            printk("ERROR: mqtt_publish %d", rc);
        } else {
            printk("Published to topic: %s", "test/topic");
        }

        k_sleep(K_SECONDS(5));
    }
}