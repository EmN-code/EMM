#include <zephyr/types.h>
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <drivers/gpio.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define WIFI_SSID "your_ssid"
#define WIFI_PASS "your_password"
#define PUBLISH_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 client_id = {.utf8 = (uint8_t*)"zephyr_client", .size = sizeof("zephyr_client") - 1};
static struct mqtt_utf8 topic = {.utf8 = (uint8_t*)"state", .size = sizeof("state") - 1};
static struct mqtt_utf8 message;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_topic subscribe_topic = {.topic = {.utf8 = (uint8_t*)"state", .size = sizeof("state") - 1}, .qos = MQTT_QOS_1_AT_LEAST_ONCE};
static struct mqtt_subscription_list subscription_list = {.list = &subscribe_topic, .list_count = 1, .message_id = 1};
static struct k_work_delayable publish_work;

static void wifi_connect(void)
{
    // Implement WiFi connection logic here
}

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT client connected!");
        break;

    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected!");
        break;

    case MQTT_EVT_PUBLISH:
        LOG_INF("MQTT message published!");
        break;

    case MQTT_EVT_PUBACK:
        LOG_INF("MQTT message acknowledged!");
        break;

    default:
        break;
    }
}

static void mqtt_setup(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id = client_id;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

static void publish_state(struct k_work *work)
{
    static bool state = false;
    state = !state;
    const char *state_str = state ? "work" : "not work";
    message.utf8 = (uint8_t *)state_str;
    message.size = strlen(state_str);

    struct mqtt_publish_param param;
    param.message.topic = topic;
    param.message.payload.data = message.utf8;
    param.message.payload.len = message.size;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    mqtt_publish(&client, &param);

    k_work_schedule(&publish_work, PUBLISH_INTERVAL);
}

void main(void)
{
    wifi_connect();
    mqtt_setup();
    k_work_init_delayable(&publish_work, publish_state);
    k_work_schedule(&publish_work, PUBLISH_INTERVAL);
}