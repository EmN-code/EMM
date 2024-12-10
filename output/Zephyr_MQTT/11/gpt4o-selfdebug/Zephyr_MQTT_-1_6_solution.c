#include <zephyr.h>
#include <net/mqtt.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_CLIENTID "zephyr_client"

static void publish_state(struct mqtt_client *client, const char *state) {
    struct mqtt_publish_param param;
    struct mqtt_utf8 topic = {
        .utf8 = "state_topic",
        .size = strlen("state_topic")
    };
    struct mqtt_utf8 payload = {
        .utf8 = (uint8_t *)state,
        .size = strlen(state)
    };
    
    param.message.topic = topic;
    param.message.payload = payload;
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message_id = sys_rand32_get();
    param.dup_flag = false;
    param.retain_flag = false;

    mqtt_publish(client, &param);
}

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from broker\n");
        break;
    default:
        break;
    }
}

void main(void) {
    struct mqtt_client client;
    uint8_t rx_buffer[128];
    uint8_t tx_buffer[128];
    struct sockaddr_in broker;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDRESS, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);

    while (1) {
        publish_state(&client, "work or not work");
        k_sleep(PUBLISH_INTERVAL);
    }
}
