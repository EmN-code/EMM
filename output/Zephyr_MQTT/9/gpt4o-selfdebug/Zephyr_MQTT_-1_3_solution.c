#include <zephyr/kernel.h>
#include <net/socket.h>
#include <mqtt.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_DBG);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_mqtt_client"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct pollfd fds[1];

static void prepare_mqtt_client(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

static void mqtt_publish_state(void)
{
    char payload[] = "work";

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)"state";
    pub_param.message.topic.topic.size = strlen("state");
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = strlen(payload);
    pub_param.message_id = sys_rand32_get();

    mqtt_publish(&client, &pub_param);
    LOG_INF("Published: %s", payload);
}

void main(void)
{
    int ret;

    prepare_mqtt_client();

    ret = mqtt_connect(&client);
    if (ret) {
        LOG_ERR("Could not connect to MQTT broker: %d", ret);
        return;
    }

    LOG_INF("MQTT client connected");

    while (1) {
        ret = mqtt_input(&client);
        if (ret) {
            LOG_ERR("mqtt_input error: %d", ret);
            continue;
        }

        mqtt_publish_state();

        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
}