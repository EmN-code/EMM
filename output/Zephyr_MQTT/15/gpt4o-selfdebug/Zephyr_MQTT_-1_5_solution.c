#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/esp32/status"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define STACKSIZE 2048
#define PRIORITY 5

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("MQTT client connected!\n");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        default:
            break;
    }
}

static void configure_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
}

void app_main(void) {
    mqtt_client_init(&client);

    configure_broker();

    struct mqtt_utf8 client_id = {
        .utf8 = (uint8_t *)MQTT_CLIENT_ID,
        .size = strlen(MQTT_CLIENT_ID)
    };

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id = client_id;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);

    while (1) {
        struct mqtt_publish_param param;
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = (uint8_t *)MQTT_PAYLOAD_WORK;
        param.message.payload.len = strlen(MQTT_PAYLOAD_WORK);
        param.message_id = 1234;
        param.dup_flag = 0;
        param.retain_flag = 0;

        mqtt_publish(&client, &param);

        k_sleep(K_SECONDS(5));
    }
}