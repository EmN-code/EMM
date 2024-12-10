#include <zephyr/sys/iterable_sections.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <net/mqtt.h>
#include <net/socket.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "zephyr_mqtt_client"

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static void publish_handler(struct mqtt_client *c, const struct mqtt_evt *evt);

static int prepare_mqtt_client(void) {
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);

    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);

    /* MQTT client configuration */
    client.broker = &broker;
    client.evt_cb = publish_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);

    return mqtt_connect(&client);
}

static void publish_handler(struct mqtt_client *c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed: %d", evt->result);
        } else {
            LOG_INF("MQTT client connected!");
        }
        break;
    case MQTT_EVT_PUBACK:
        if (evt->result != 0) {
            LOG_ERR("Publish failed: %d", evt->result);
        } else {
            LOG_INF("Publish acknowledged!");
        }
        break;
    default:
        break;
    }
}

static void mqtt_publish_message(struct mqtt_client *c) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"work/status";
    param.message.topic.topic.size = strlen("work/status");
    param.message.payload.data = (uint8_t *)(k_uptime_ticks() % 2 == 0 ? "work" : "not work");
    param.message.payload.len = strlen(param.message.payload.data);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(client, &param);
    if (rc != 0) {
        LOG_ERR("Failed to publish message: %d", rc);
    }
}

void main(void) {
    int rc;

    LOG_INF("Starting MQTT publisher sample");

    rc = prepare_mqtt_client();
    if (rc != 0) {
        LOG_ERR("Failed to prepare MQTT client: %d", rc);
        return;
    }

    while (1) {
        mqtt_publish_message(&client);
        k_sleep(PUBLISH_INTERVAL);
    }
}