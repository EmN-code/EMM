#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define MQTT_BROKER   "47.102.103.1"
#define MQTT_PORT     1883
#define MQTT_CLIENTID "esp32_client"
#define TOPIC         "esp32/state"
#define STATE_PAYLOAD "work or not work"

struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        default:
            break;
    }
}

int main(void) {
    struct sockaddr_in broker;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_PORT);
    inet_pton(AF_INET, MQTT_BROKER, &broker.sin_addr);

    struct mqtt_sec_config sec_cfg = {
        .peer_verify = 0,
    };

    const struct mqtt_client_init_params init_params = {
        .broker = &broker,
        .evt_cb = mqtt_event_handler,
        .client_id.utf8 = (uint8_t *)MQTT_CLIENTID,
        .client_id.size = strlen(MQTT_CLIENTID),
        .user_name = NULL,
        .password = NULL,
        .protocol_version = MQTT_VERSION_3_1_1,
        .rx_buffer = rx_buffer,
        .rx_buffer_size = sizeof(rx_buffer),
        .tx_buffer = tx_buffer,
        .tx_buffer_size = sizeof(tx_buffer),
    };

    mqtt_init(&client, &init_params);
    if (mqtt_connect(&client) != 0) {
        printk("MQTT connect failed\n");
        return -1;
    }

    while (1) {
        struct mqtt_publish_param param = {
            .message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE,
            .message.topic.topic.utf8 = (uint8_t *)TOPIC,
            .message.topic.topic.size = strlen(TOPIC),
            .message.payload.data = (uint8_t *)STATE_PAYLOAD,
            .message.payload.len = strlen(STATE_PAYLOAD),
            .message_id = 1,
            .dup_flag = 0,
            .retain_flag = 0
        };

        mqtt_publish(&client, &param);

        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
    return 0;
}