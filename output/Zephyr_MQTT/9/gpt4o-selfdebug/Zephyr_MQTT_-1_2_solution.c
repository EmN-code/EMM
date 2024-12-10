#include <zephyr.h>
#include <sys/printk.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_client"
#define STACKSIZE 1024

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct sockaddr_in broker;
static const char *topic = "device/status";

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected\n");
            break;
        default:
            break;
    }
}

void configure_mqtt(void) {
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void mqtt_publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = (uint8_t *)("work");
    param.message.payload.len = strlen("work");
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void) {
    configure_mqtt();

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect MQTT client: %d\n", rc);
        return;
    }

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }
}