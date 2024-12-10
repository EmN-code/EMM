#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

/* MQTT client configuration */
static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static bool connected = false;

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                connected = true;
            }
            break;
        case MQTT_EVT_DISCONNECT:
            connected = false;
            break;
        default:
            break;
    }
}

static int mqtt_connect(void) {
    struct sockaddr_in *broker_in = &broker;

    broker_in->sin_family = AF_INET;
    broker_in->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_in->sin_addr);

    struct mqtt_sec_config sec_cfg = { 0 };

    struct mqtt_utf8 client_id = { .utf8	= "client_id",
                                   .size	= strlen("client_id") };

    mqtt_client_init(&client);

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    return mqtt_connect(&client);
}

void main(void) {
    int ret;

    ret = mqtt_connect();
    if (ret) {
        printk("Failed to connect to broker, error: %d\n", ret);
        return;
    }

    while (1) {
        if (connected) {
            struct mqtt_publish_param publish_param;
            publish_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
            publish_param.message.topic.topic.utf8 = "state";
            publish_param.message.topic.topic.size = strlen("state");
            publish_param.message.payload.data = "work";
            publish_param.message.payload.len = strlen("work");

            ret = mqtt_publish(&client, &publish_param);
            if (ret) {
                printk("Failed to publish message, error: %d\n", ret);
            }
        }

        k_sleep(PUBLISH_INTERVAL);
    }
}