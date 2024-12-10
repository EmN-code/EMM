#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_sample, LOG_LEVEL_DBG);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define APP_SLEEP_MSECS 5000

static struct mqtt_client client;
static struct sockaddr broker;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static uint8_t payload[] = "work";
static struct mqtt_utf8 topic = MQTT_UTF8_LITERAL("state");

static void prepare_fds(struct mqtt_client *client, struct pollfd *fds)
{
    fds->fd = client->transport.tcp.sock;
    fds->events = ZSOCK_POLLIN;
}

static int wait(int timeout)
{
    struct pollfd fds;
    int ret;

    prepare_fds(&client, &fds);

    ret = poll(&fds, 1, timeout);
    if (ret < 0) {
        LOG_ERR("Error in poll: %d", errno);
    }

    return ret;
}

static int publish_data(struct mqtt_client *client)
{
    struct mqtt_publish_param param;

    param.message.topic = topic;
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    LOG_INF("Publishing data to topic %s", log_strdup(topic.utf8));

    return mqtt_publish(client, &param);
}

static void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

void main(void)
{
    int rc;

    mqtt_client_init(&client);

    broker_init();

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)"zephyr_publisher";
    client.client_id.size = strlen("zephyr_publisher");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("Could not connect to MQTT broker: %d", rc);
        return;
    }

    while (1) {
        rc = mqtt_input(&client);
        if (rc != 0) {
            LOG_ERR("Error in mqtt_input: %d", rc);
            goto exit;
        }

        rc = mqtt_live(&client);
        if (rc != 0) {
            LOG_ERR("Error in mqtt_live: %d", rc);
            goto exit;
        }

        /* Publish state every APP_SLEEP_MSECS milliseconds */
        publish_data(&client);

        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }

exit:
    mqtt_disconnect(&client);
}