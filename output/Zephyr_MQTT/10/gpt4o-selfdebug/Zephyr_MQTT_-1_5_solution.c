#include <zephyr.h>
#include <net/mqtt.h>

#define STACK_SIZE 1024
#define PRIORITY 7

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static bool connected = false;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            connected = true;
            printk("Connected to MQTT broker\n");
            break;

        case MQTT_EVT_DISCONNECT:
            connected = false;
            printk("Disconnected from MQTT broker\n");
            break;
        
        default:
            break;
    }
}

void init_mqtt_client(void) {
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_TYPE_TCP;
    client.protocol_version = MQTT_VERSION_3_1_1;
}

void connect_to_mqtt(void) {
    struct mqtt_sec_config sec_config;
    memset(&sec_config, 0, sizeof(sec_config));

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("mqtt_connect failed: %d\n", rc);
    }
}

void publish_state_work(void) {
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = "work";
    param.message.payload.len = strlen("work");
    param.message_id = sys_rand32_get();

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        printk("mqtt_publish failed: %d\n", rc);
    }
}

void main(void) {
    printk("Starting MQTT Work State Publisher\n");

    init_mqtt_client();
    connect_to_mqtt();

    while (1) {
        if (connected) {
            publish_state_work();
        }
        k_sleep(K_SECONDS(5));
    }
}