
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_client_config config;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_client *client, const char *state)
{
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Published state: %s", state);
    }
}

void main(void)
{
    int sock;
    int ret;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&config, 0, sizeof(config));

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    ret = connect(sock, (struct sockaddr *)&broker, sizeof(broker));
    if (ret < 0) {
        LOG_ERR("Failed to connect to broker: %d", ret);
        close(sock);
        return;
    }

    config.client_id.utf8 = MQTT_CLIENT_ID;
    config.client_id.size = strlen(MQTT_CLIENT_ID);
    config.user_name.utf8 = MQTT_USERNAME;
    config.password.utf8 = MQTT_PASSWORD;
    config.keepalive = 60;
    config.clean_session = true;
    config.store = NULL;
    config.evt_cb = mqtt_evt_handler;

    mqtt_client_init(&client_ctx);
    ret = mqtt_connect(&client_ctx, &config, sock, rx_buffer, sizeof(rx_buffer), tx_buffer, sizeof(tx_buffer));
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        close(sock);
        return;
    }

    while (1) {
        publish_state(&client_ctx, "work");
        k_sleep(K_SECONDS(5));
        publish_state(&client_ctx, "not work");
        k_sleep(K_SECONDS(5));
    }
}
