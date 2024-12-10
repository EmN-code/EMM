#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

const struct mqtt_utf8 client_id = MQTT_UTF8_INITIALIZER("esp32_client");
const struct mqtt_utf8 topic = MQTT_UTF8_INITIALIZER("state");

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_client client;
static struct sockaddr_in broker;

void broker_init(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    /* Handle various MQTT events here */
}

void mqtt_init(void) {
    struct mqtt_sec_config sec_config;

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id = client_id;
    client.password = NULL;
    client.user_name = NULL;
    client.clean_session = 1;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

void publish_state(bool state) {
    struct mqtt_publish_param param;
    const char *message = state ? "work" : "not work";

    param.message.topic = topic;
    param.message.payload.data = message;
    param.message.payload.len = strlen(message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_publish(&client, &param);
}

void main(void) {
    broker_init();
    mqtt_init();

    bool state = true;

    while (1) {
        publish_state(state);
        state = !state;
        k_sleep(PUBLISH_INTERVAL);
    }
}