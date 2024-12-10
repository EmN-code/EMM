#include <zephyr/kernel.h>
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <device.h>
#include <drivers/gpio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_CLIENTID "zephyr_client"
#define MQTT_QOS 1

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBLISH:
            printk("MQTT PUBLISH event received!");
            break;
        default:
            printk("Unknown MQTT event type: %d", evt->type);
            break;
    }
}

static void mqtt_configure(void)
{
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);

    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = NULL;
    client.rx_buf_size = 0;
    client.tx_buf = NULL;
    client.tx_buf_size = 0;
}

void publish_state(void)
{
    char payload[] = "work or not work";
    pub_param.message.topic.qos = MQTT_QOS;
    pub_param.message.topic.topic.utf8 = "state";
    pub_param.message.topic.topic.size = strlen("state");
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = strlen(payload);
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0U;
    pub_param.retain_flag = 0U;

    mqtt_publish(&client, &pub_param);
    printk("Publishing state: %s\n", payload);
}

void main(void)
{
    printk("Starting MQTT sample\n");
    mqtt_configure();

    if (mqtt_connect(&client)) {
        printk("MQTT connect failed\n");
        return;
    }

    while (1) {
        publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}