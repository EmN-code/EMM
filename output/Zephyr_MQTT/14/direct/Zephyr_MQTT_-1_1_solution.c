
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define STACK_SIZE 1024
#define PRIORITY 7

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 topic;
static struct mqtt_utf8 payload;
static struct sockaddr_in broker;

void publish_state(struct mqtt_client *client) {
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    payload.utf8 = state_str;
    payload.size = strlen(state_str);

    int ret = mqtt_publish(client, &topic, &payload, MQTT_QOS_0_AT_MOST_ONCE, false);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Published: %s", state_str);
    }
}

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("Connected to MQTT broker");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("Disconnected from MQTT broker");
            break;
        default:
            break;
    }
}

void mqtt_thread(void) {
    int sock;
    struct mqtt_client_config config;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&config, 0, sizeof(config));

    config.client_id.utf8 = MQTT_CLIENT_ID;
    config.client_id.size = strlen(MQTT_CLIENT_ID);
    config.user_name.utf8 = MQTT_USERNAME;
    config.password.utf8 = MQTT_PASSWORD;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        LOG_ERR("Failed to connect to broker");
        close(sock);
        return;
    }

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_cfg = &config;
    client_ctx.sock = sock;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        close(sock);
        return;
    }

    while (1) {
        mqtt_input(&client_ctx);
        publish_state(&client_ctx);
        k_sleep(K_SECONDS(5));
    }
}

K_THREAD_DEFINE(mqtt_thread_id, STACK_SIZE, mqtt_thread, NULL, NULL, NULL, PRIORITY, 0, 0);

int main(void) {
    LOG_INF("Starting MQTT client");
    return 0;
}
