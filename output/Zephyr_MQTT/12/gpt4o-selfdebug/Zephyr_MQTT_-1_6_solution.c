#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define SERVER_ADDR   "47.102.103.1"
#define SERVER_PORT   1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buffer[128];

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
        printk("Received MQTT publish message: %s", log_strdup((const char *)evt->param.publish.message.payload.data));
        break;
    default:
        break;
    }
}

static int publish_state(struct mqtt_client *c)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"esp32/state";
    param.message.topic.topic.size = strlen("esp32/state");
    param.message.payload.data = payload_buffer;
    param.message.payload.len = strlen(payload_buffer);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    strncpy(payload_buffer, "work", sizeof(payload_buffer) - 1);

    return mqtt_publish(c, &param);
}

void main(void)
{
    struct sockaddr_in broker;

    memset(&client, 0x00, sizeof(client));
    memset(&broker, 0x00, sizeof(broker));

    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    zsock_inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;

    mqtt_client_init(&client);

    while (1) {
        if (mqtt_connect(&client) == 0) {
            LOG_INF("Connected to MQTT broker");

            while (1) {
                publish_state(&client);
                k_sleep(PUBLISH_INTERVAL);
            }
        } else {
            LOG_ERR("Failed to connect to MQTT broker");
            k_sleep(K_SECONDS(5));
        }
    }
}