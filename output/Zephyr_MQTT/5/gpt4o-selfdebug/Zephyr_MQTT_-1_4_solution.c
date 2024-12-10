#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/random/rand32.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param param;

static char payload[] = "work";

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected!");
        break;
    default:
        LOG_INF("MQTT event: %d", evt->type);
        break;
    }
}

void connect_to_broker(void)
{
    struct mqtt_sec_config *tls_config = NULL;
    struct mqtt_utf8 username = { .utf8 = NULL, .size = 0 };
    struct mqtt_utf8 password = { .utf8 = NULL, .size = 0 };

    struct mqtt_connect_msg connect_msg = {
        .user_name = &username,
        .password = &password,
        .client_id = MQTT_CLIENTID_DEFAULT,
        .clean_session = 1,
        .will_topic = NULL,
        .will_payload = NULL,
        .will_qos = MQTT_QOS_0_AT_MOST_ONCE,
    };

    int rc = mqtt_connect(&client, (struct sockaddr *)&broker,
                          sizeof(struct sockaddr_in), tls_config, K_MSEC(2000), &connect_msg);
    if (rc != 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", rc);
    }
}

void mqtt_publish_state(struct k_work *work)
{
    ARG_UNUSED(work);

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "esp32/state";
    param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        LOG_ERR("Failed to publish message: %d", rc);
    }
}

K_WORK_DEFINE(publish_work, mqtt_publish_state);

void main(void)
{
    LOG_INF("Starting MQTT publisher...");

    broker.sin_family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);
    broker.sin_port = htons(MQTT_BROKER_PORT);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "esp32_mqtt_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.protocol_version = MQTT_VERSION_3_1_1;

    connect_to_broker();

    while (1) {
        k_work_submit(&publish_work);
        k_sleep(PUBLISH_INTERVAL);
    }
}