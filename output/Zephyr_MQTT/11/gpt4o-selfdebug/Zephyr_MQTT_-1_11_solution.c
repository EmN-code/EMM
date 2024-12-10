#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_mqtt_client"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected");
            break;
        case MQTT_EVT_PUBACK:
            LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);
            break;
        default:
            break;
    }
}

static int prepare_fds(struct mqtt_client *c)
{
    if (c->transport.type == MQTT_TRANSPORT_NON_SECURE) {
        ((struct sockaddr_in *)&broker)->sin_family = AF_INET;
        ((struct sockaddr_in *)&broker)->sin_port = htons(BROKER_PORT);
        net_addr_pton(AF_INET, BROKER_ADDR,
                      &((struct sockaddr_in *)&broker)->sin_addr);
    }
    return 0;
}

static int init_mqtt_client(void)
{
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;

    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    prepare_fds(&client);

    return mqtt_connect(&client);
}

static int publish_msg(const char *topic, const char *payload)
{
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)topic;
    pub_param.message.topic.topic.size = strlen(topic);

    pub_param.message.payload.data = (uint8_t *)payload;
    pub_param.message.payload.len = strlen(payload);

    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    return mqtt_publish(&client, &pub_param);
}

void main(void)
{
    int ret;

    LOG_INF("Starting MQTT sample");

    ret = init_mqtt_client();
    if (ret) {
        LOG_ERR("Failed to initialize MQTT client: %d", ret);
        return;
    }

    while (1) {
        ret = mqtt_input(&client);
        if (ret < 0) {
            LOG_ERR("MQTT input error: %d", ret);
        }

        ret = mqtt_live(&client);
        if (ret < 0) {
            LOG_ERR("MQTT live error: %d", ret);
        }

        publish_msg("esp32/status", "work");
        k_sleep(PUBLISH_INTERVAL);
    }
    mqtt_disconnect(&client);
}