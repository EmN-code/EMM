#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <net/tls_credentials.h>

#define BROKER_ADDR  "47.102.103.1"
#define BROKER_PORT  1883
#define APP_CONNECT_TRIES 3
#define APP_SLEEP_MSECS   5000

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct sockaddr_storage broker;

static struct mqtt_client client;

void prepare_fds(struct mqtt_client *client);
void clear_fds(void);

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to the broker\n");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from the broker\n");
        break;

    default:
        break;
    }
}

int init_mqtt_client(void)
{
    mqtt_client_init(&client);

    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &net_sin(&broker)->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    client.protocol_version = MQTT_VERSION_3_1_1;

    return mqtt_connect(&client);
}

void main(void)
{
    int err;

    err = init_mqtt_client();
    if (err != 0) {
        printk("Failed to connect to broker, error: %d
", err);
        return;
    }

    while (1) {
        printk("Publishing state to broker…\n");

        struct mqtt_publish_param param;

        param.message.topic.topic.s = "device/state";
        param.message.topic.topic.size = strlen("device/state");

        param.message.payload.data = "work";
        param.message.payload.len = strlen("work");

        param.message_id = sys_rand32_get();
        param.dup_flag = 0;
        param.retain_flag = 0;

        /* Attempt to publish the message */
        err = mqtt_publish(&client, &param);
        if (err) {
            printk("Failed to publish message, error: %d
", err);
        } else {
            printk("Message published
");
        }

        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }
}