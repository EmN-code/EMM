#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "esp32/state"
#define PUBLISH_INTERVAL K_SECONDS(5)

struct mqtt_client client;
struct sockaddr_storage broker;
uint8_t rx_buffer[128];
uint8_t tx_buffer[128];
uint8_t payload[] = "work or not work";

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to MQTT broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT message published\n");
        break;
    default:
        break;
    }
}

void connect_to_mqtt(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    struct mqtt_sec_config tls_config = {
        .peer_verify = 0,
        .cipher_list = NULL,
        .sec_tag_list = NULL,
        .sec_tag_count = 0,
        .hostname = MQTT_BROKER_ADDR,
    };

    struct mqtt_ipv4_broker srv_addr = {
        .broker = { .addr = *broker4 },
        .port = MQTT_BROKER_PORT
    };

    struct mqtt_client_init_param init = {
        .client_id.utf8 = MQTT_CLIENT_ID,
        .client_id.size = strlen(MQTT_CLIENT_ID),
        .protocol_version = MQTT_VERSION_3_1_1,
        .evt_cb = mqtt_event_handler,
        .client = &client,
        .tx = tx_buffer,
        .tx_size = sizeof(tx_buffer),
        .rx = rx_buffer,
        .rx_size = sizeof(rx_buffer),
        .srv_addr = srv_addr,
        .tls_config = tls_config
    };

    mqtt_client_init(&init);
    mqtt_connect(&client);
}

void mqtt_publish_message(void)
{
    struct mqtt_publish_param param;

    param.message_id = 1;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void)
{
    printk("Initializing MQTT client\n");
    connect_to_mqtt();

    while (1) {
        mqtt_publish_message();
        k_sleep(PUBLISH_INTERVAL);
    }
}