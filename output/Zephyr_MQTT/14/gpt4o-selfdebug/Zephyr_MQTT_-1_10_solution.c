#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = "work or not work";
static struct mqtt_publish_param pub_param;

static void prepare_fds(struct mqtt_client *client)
{
    if (client->transport.type == MQTT_TRANSPORT_NON_SECURE) {
        client->transport.tcp.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
}

static void connect_broker(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    int rc = connect(client.transport.tcp.sock, (struct sockaddr *)broker4, sizeof(struct sockaddr_in));
    if (rc != 0) {
        LOG_ERR("Failed to connect to broker: %d", rc);
        return;
    }

    LOG_INF("Connected to broker");
}

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT connected");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT disconnected");
        break;
    case MQTT_EVT_PUBLISH:
        LOG_INF("MQTT publish received");
        break;
    default:
        break;
    }
}

void main(void)
{
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_publisher";
    client.client_id.size = strlen("zephyr_publisher");
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    prepare_fds(&client);
    connect_broker();

    struct mqtt_sec_config *tls_cfg = &client.transport.tls.config;
    tls_cfg->peer_verify = 2;
    tls_cfg->cipher_list = NULL;
    tls_cfg->cipher_count = 0;
    tls_cfg->sec_tag_list = NULL;
    tls_cfg->sec_tag_count = 0;
    tls_cfg->hostname = NULL;

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("MQTT connect failed: %d", rc);
        return;
    }

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)"zephyr/test";
    pub_param.message.topic.topic.size = strlen("zephyr/test");
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = sizeof(payload) - 1;
    pub_param.message_id = 0;
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);

        rc = mqtt_publish(&client, &pub_param);
        if (rc != 0) {
            LOG_ERR("Failed to publish message: %d", rc);
        } else {
            LOG_INF("Message published");
        }

        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
    close(client.transport.tcp.sock);
}