#include <zephyr/kernel.h>
#include <posix/netinet/in.h>
#include <posix/errno.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define MQTT_CLIENTID       "esp32_mqtt"
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUBLISH_INTERVAL    K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 client_id = { .utf8 = MQTT_CLIENTID, .size = sizeof(MQTT_CLIENTID) - 1 };
static char tx_buffer[128];
static char rx_buffer[128];

static void mqtt_event_handler(struct mqtt_client *c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("Incoming publish message\n");
            break;
        default:
            break;
    }
}

static int connect_mqtt_broker(void) {
    int rc;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    net_addr_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker, error %d\n", rc);
        return rc;
    }

    return 0;
}

static void publish_state(const char *state) {
    struct mqtt_publish_param param;
    param.message.payload.data = (uint8_t *)state;
    param.message.payload.len = strlen(state);
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        printk("Failed to publish message, error %d\n", rc);
    }
}

void main(void) {
    int rc;

    printk("Connecting to MQTT broker...\n");
    rc = connect_mqtt_broker();
    if (rc != 0) {
        return;
    }

    while (1) {
        rc = mqtt_input(&client);
        if (rc != 0) {
            printk("Error reading from MQTT broker, error %d\n", rc);
        }

        publish_state("work");
        k_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
        k_sleep(PUBLISH_INTERVAL);
    }
}