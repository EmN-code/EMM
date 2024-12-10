#include <zephyr/kernel.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <stdio.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_utf8 topic = {.utf8 = "device/state", .size = 12};
static struct mqtt_utf8 message_work = {.utf8 = "work", .size = 4};
static struct mqtt_utf8 message_not_work = {.utf8 = "not work", .size = 8};

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    ARG_UNUSED(c);
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!\n");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT PUBLISH event\n");
        break;
    case MQTT_EVT_PUBACK:
        printk("MQTT PUBACK event\n");
        break;
    default:
        break;
    }
}

void mqtt_setup(void)
{
    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &net_sin(&broker)->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    struct mqtt_secure_channel *channel = &client.transport.tls;
    channel->tls_type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

void main(void)
{
    printf("Starting MQTT sample\n");

    mqtt_setup();

    while (1) {
        mqtt_live(&client);
        mqtt_input(&client);

        if (mqtt_publish(&client, message_work.utf8, message_work.size) != 0) {
            printk("Failed to publish work state\n");
        }

        k_sleep(K_SECONDS(5));
    }
}