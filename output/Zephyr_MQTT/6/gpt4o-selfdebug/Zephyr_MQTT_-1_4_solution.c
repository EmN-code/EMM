#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = "work or not work";

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("MQTT message published!\n");
            break;
        default:
            break;
    }
}

void configure_mqtt_client(void) {
    struct mqtt_utf8 client_id = { .utf8 = (uint8_t *)MQTT_CLIENTID, .size = strlen(MQTT_CLIENTID) };
    struct mqtt_utf8 user_name = { .utf8 = (uint8_t *)MQTT_USERNAME, .size = strlen(MQTT_USERNAME) };
    struct mqtt_utf8 password = { .utf8 = (uint8_t *)MQTT_PASSWORD, .size = strlen(MQTT_PASSWORD) };

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.user_name = (MQTT_USERNAME != NULL) ? &user_name : NULL;
    client.password = (MQTT_PASSWORD != NULL) ? &password : NULL;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void configure_broker(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
}

void mqtt_publish_task(void) {
    struct mqtt_publish_param param;

    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    while (1) {
        mqtt_publish(&client, &param);
        k_sleep(PUBLISH_INTERVAL);
    }
}

void main(void) {
    configure_broker();
    configure_mqtt_client();

    if (mqtt_connect(&client)) {
        printk("Failed to connect to MQTT broker\n");
        return;
    }

    mqtt_publish_task();
}