#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"
#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_publish_param pub_param;

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("Connected to MQTT broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVT_PUBACK:
            printk("MQTT message published\n");
            break;
        default:
            break;
    }
}

void connect_to_broker() {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker (%d)\n", rc);
    }
}

void publish_state() {
    pub_param.message.topic.topic.utf8 = (unsigned char *)TOPIC;
    pub_param.message.topic.topic.size = strlen(TOPIC);
    pub_param.message.payload.data = (unsigned char *)"work";
    pub_param.message.payload.len = strlen("work");
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    while (1) {
        mqtt_publish(&client, &pub_param);
        k_sleep(K_SECONDS(5));
    }
}

int main(void) {
    connect_to_broker();
    publish_state();
    return 0;
}
