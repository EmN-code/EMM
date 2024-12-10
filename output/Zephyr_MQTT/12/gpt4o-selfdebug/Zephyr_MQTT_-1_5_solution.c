#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "state_topic"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[128];

static struct pollfd fds;
static bool connected;

static void prepare_fds(void)
{
    fds.fd = client.transport.tcp.sock;
    fds.events = POLLIN;
}

static int wait(int timeout)
{
    int ret = poll(&fds, 1, timeout);

    if (ret < 0) {
        LOG_ERR("poll error: %d", errno);
        return -errno;
    }

    return ret;
}

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
            break;
        }
        LOG_INF("MQTT client connected!");
        connected = true;
        break;

    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected %d", evt->result);
        connected = false;
        break;

    default:
        break;
    }
}

static int publish_message(struct mqtt_client *client)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_PUB_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
    param.message.payload.data = (uint8_t *)"work";
    param.message.payload.len = strlen("work");
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    LOG_INF("Publishing: %s", "work");

    return mqtt_publish(client, &param);
}

static void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);

    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

static void client_init(struct mqtt_client *client)
{
    mqtt_client_init(client);

    broker_init();

    client->broker = &broker;
    client->evt_cb = mqtt_event_handler;
    client->client_id.utf8 = (uint8_t *)"zephyr_client";
    client->client_id.size = strlen("zephyr_client");
    client->password = NULL;
    client->user_name = NULL;
    client->protocol_version = MQTT_VERSION_3_1_1;

    client->rx_buf = rx_buffer;
    client->rx_buf_size = sizeof(rx_buffer);
    client->tx_buf = tx_buffer;
    client->tx_buf_size = sizeof(tx_buffer);
}

void main(void)
{
    int rc;

    LOG_INF("Starting MQTT client sample");

    client_init(&client);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("mqtt_connect failed: %d", rc);
        return;
    }

    prepare_fds();

    while (1) {
        if (!connected) {
            continue;
        }

        rc = publish_message(&client);
        if (rc != 0) {
            LOG_ERR("Publish failed: %d", rc);
        }

        rc = wait(PUBLISH_INTERVAL);
        if (rc < 0) {
            LOG_ERR("Wait error: %d", rc);
        }

        mqtt_input(&client);
        mqtt_live(&client);
    }
}